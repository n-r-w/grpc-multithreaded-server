#include "sl_server.h"
#include "sl_utils.h"

namespace sl
{
ServiceFactory* Server::_service_factory_global = nullptr;

Server::Server(ServiceFactory* service_factory)
    : _service_factory(service_factory)
{
    assert(_service_factory != nullptr);
    assert(_service_factory_global == nullptr);
    _service_factory_global = _service_factory;
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

void Server::start(const std::string& server_address, size_t thread_count)
{
    assert(thread_count > 0);
    assert(_thread == nullptr);

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

void Server::process()
{
    _builder = std::make_unique<grpc::ServerBuilder>();

    grpc::ResourceQuota quota;
    quota.SetMaxThreads(_thread_count);
    _builder->SetResourceQuota(quota);

    _builder->AddListeningPort(_server_address, grpc::InsecureServerCredentials());

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
