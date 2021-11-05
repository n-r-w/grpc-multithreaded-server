#include "sl_request_worker.h"
#include "sl_request_processor.h"
#include "sl_utils.h"

namespace sl
{
RequestWorker::RequestWorker(grpc::ServerCompletionQueue* queue, UserValidator* user_validator)
    : _queue(queue)
    , _user_validator(user_validator)
{
    assert(_queue != nullptr);
}

RequestWorker::~RequestWorker()
{
    stopRequest();
    stopWait();
}

void RequestWorker::start()
{
    assert(_thread == nullptr);
    assert(!_stopped);

    // запускаем поток обработки
    _thread = new std::thread([&]() { process(); });
}

void RequestWorker::stopWait()
{
    if (_thread == nullptr)
        return;

    // ждем завершения
    _thread->join();
    delete _thread;
    _thread = nullptr;

    Utils::coutPrint("request worker stopped");
}

void RequestWorker::runRequestProcessorsPool() const
{
    for (auto& key : ServiceFactory::instance()->serviceKeys()) {
        auto processors = createRequestProcessors(key);
        for (auto p : processors) {
            p->run(grpc::StatusCode::OK);
        }
    }
}

grpc::ServerCompletionQueue* RequestWorker::queue() const
{
    return _queue;
}

void RequestWorker::process()
{
    runRequestProcessorsPool();

    void* tag;
    bool ok;
    while (!isStopRequested()) {
        grpc::CompletionQueue::NextStatus next_status
            = queue()->AsyncNext(&tag, &ok, std::chrono::system_clock::now() + std::chrono::milliseconds(100));

        if (next_status == grpc::CompletionQueue::NextStatus::GOT_EVENT) {
            auto processor = static_cast<sl::RequestProcessorBase*>(tag);

            std::string login;
            std::string password;

            UservValidationResult v_res = extractUserValidationInfo(processor->service(), processor->context(), login, password);

            if (v_res == UservValidationResult::NotValidated) {
                processor->run(grpc::StatusCode::INVALID_ARGUMENT);

            } else {
                if (v_res == UservValidationResult::Validated && !_user_validator->checkUser(login, password))
                    processor->run(grpc::StatusCode::UNAUTHENTICATED);
                else
                    processor->run(grpc::StatusCode::OK);
            }
        }
    }
}

void RequestWorker::stopRequest()
{
    if (_thread == nullptr)
        return;

    _stopped = true;
    // запрашиваем остановку
    StoppableWorker::stopRequest();
    Utils::coutPrint("request worker stop requested");
}

} // namespace hrs
