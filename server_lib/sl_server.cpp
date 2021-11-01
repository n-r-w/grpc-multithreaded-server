#include "sl_server.h"
#include "sl_utils.h"
#include <fstream>

namespace sl
{
ServiceFactory* Server::_service_factory_global = nullptr;
UserValidator* Server::_user_validator_global = nullptr;

Server::Server(ServiceFactory* service_factory, UserValidator* user_validator)
    : _service_factory(service_factory)
    , _user_validator(user_validator)
{
    assert(_service_factory != nullptr);
    assert(_service_factory_global == nullptr);

    assert(_user_validator != nullptr);
    assert(_user_validator_global == nullptr);

    _service_factory_global = _service_factory;
    _user_validator_global = _user_validator;
}

Server::~Server()
{
    stop();
}

ServiceFactory* Server::serviceFactory()
{
    assert(_service_factory_global != nullptr);
    return _service_factory_global;
}

UserValidator* Server::userValidator()
{
    assert(_user_validator_global != nullptr);
    return _user_validator_global;
}

void Server::start(const std::string& server_address, size_t thread_count, const std::string& root_cert, const std::string& ssl_cert,
                   const std::string& ssl_key)
{
    assert(thread_count > 0);
    assert(_thread == nullptr);

    _root_cert = root_cert;
    _ssl_cert = ssl_cert;
    _ssl_key = ssl_key;

    _server_address = server_address;
    _thread_count = thread_count;

    // запускаем поток обработки
    _thread = new std::thread([this]() { process(); });
}

void Server::stop()
{
    if (_thread == nullptr)
        return;

    // запрашиваем остановку
    stopRequest();

    Utils::coutPrint("server worker stopping...");
    // ждем завершения
    _thread->join();
    Utils::coutPrint("server worker stopped");
    delete _thread;
    _thread = nullptr;

    Utils::coutPrint("server shutdowning...");
    _server->Shutdown(
        // под нагрузкой не желает останавливаться пока все не обработает, а запросы не дают ему это сделать
        std::chrono::system_clock::now() + std::chrono::seconds(3));

    if (_queue != nullptr) {
        Utils::coutPrint("queues shutdowning...");
        _queue->Shutdown();

        // ждем очистки очередей, иначе сервер будет ругаться при очистке ресурсов
        Utils::coutPrint("queue cleaning...");
        bool ok;
        void* got_tag;
        while (_queue->Next(&got_tag, &ok)) {
        }
    }
}

static std::string read_keycert(const char* filename)
{
    std::ifstream ifs(filename);
    std::string content;
    content.assign((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
    return content;
}

void Server::process()
{
    _builder = std::make_unique<grpc::ServerBuilder>();

    grpc::ResourceQuota quota;
    quota.SetMaxThreads(_thread_count);
    _builder->SetResourceQuota(quota);

    // параметры безопасности
    std::shared_ptr<grpc::ServerCredentials> creds;
    if (!_ssl_key.empty() && !_ssl_cert.empty()) {
        grpc::SslServerCredentialsOptions::PemKeyCertPair pkcp;
        pkcp.private_key = _ssl_key;
        pkcp.cert_chain = _ssl_cert;

        grpc::SslServerCredentialsOptions ssl_opts;

#ifdef GPR_WINDOWS // https://grpc.io/docs/guides/auth/
        ssl_opts.pem_root_certs = _root_cert;
#endif
        ssl_opts.pem_key_cert_pairs.push_back(pkcp);

        creds = grpc::SslServerCredentials(ssl_opts);

    } else {
        creds = grpc::InsecureServerCredentials();
    }

    // устанавливаем порт и безопасность
    _builder->AddListeningPort(_server_address, creds);

    for (auto& key : _service_factory->serviceKeys()) {
        _builder->RegisterService(_service_factory->getService(key));
    }

    // создаем очереди
    _queue = std::move(_builder->AddCompletionQueue());

    // Создаем обработчики
    for (size_t i = 0; i < _thread_count; i++) {
        auto worker = createWorker(_queue.get());
        assert(worker != nullptr);
        _workers.push_back(worker);
    }

    _server = _builder->BuildAndStart();
    Utils::coutPrint("Server listening on " + _server_address + ". Thread count " + std::to_string(_thread_count));

    for (auto& w : _workers) {
        w->start();
    }

    while (!isStopRequested()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    for (auto& w : _workers) {
        w->stopRequest();
    }

    for (auto& w : _workers) {
        w->stopWait();
    }

    for (auto& w : _workers) {
        delete w;
    }
    _workers.clear();
}

} // namespace hrs
