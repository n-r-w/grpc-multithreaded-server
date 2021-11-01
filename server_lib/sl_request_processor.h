#pragma once

#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>
#include "sl_utils.h"
#include "sl_service_factory.h"

// clang-format off
#define SL_DEFINE_PROCESSOR_HELPER(ClassName,ServiceType,RequestType,ReplyType,Method) \
class ClassName : public sl::RequestProcessor<ServiceType, RequestType, ReplyType> \
{ \
    public: \
    ClassName(grpc::ServerCompletionQueue* queue, const sl::RequestWorker* worker) \
    : sl::RequestProcessor<ServiceType, RequestType, ReplyType>(queue, worker, HrsServiceFactory::TEST_SERVICE_KEY) {} \
    protected: \
    void waitForRequest() override { service()->Method (context(), request(), responder(), queue(), queue(), this); } \
    ClassName* createRequestProcessor() override { return new ClassName(queue(), worker()); } \
    void handleRequest() override; \
}; \

// Макро для создания класса обработчика метода сервиса
// Кроме самого макро надо создать метод обработчик запроса
// void ClassName::handleRequest() {...}
#define SL_DEFINE_PROCESSOR(ClassName,ServiceType,RequestType,ReplyType,Method) SL_DEFINE_PROCESSOR_HELPER(ClassName,ServiceType,RequestType,ReplyType,Request##Method)
// clang-format on

namespace sl
{
class RequestWorker;

class RequestProcessorBase
{
public:
    RequestProcessorBase() { }
    virtual void run(grpc::StatusCode code, const std::string& error_message = {}) = 0;
    //! Контекст запроса
    virtual grpc::ServerContext* context() const = 0;

protected:
    virtual void waitForRequest() = 0;
    virtual void handleRequest() = 0;
};

template <class ServiceType, class RequestType, class ReplyType> class RequestProcessor : public RequestProcessorBase
{
public:
    RequestProcessor(
        //! Очередь
        grpc::ServerCompletionQueue* queue,
        //! Обработчик потока
        const RequestWorker* worker,
        //! Идентификатор сервиса
        const std::string& service_key)
        : _queue(queue)
        , _worker(worker)
        , _service(static_cast<ServiceType*>(ServiceFactory::instance()->getService(service_key)))
        , _responder(&_context)

    {
        assert(_queue != nullptr);
        assert(_worker != nullptr);        
        assert(_service != nullptr);
    }

    void run(grpc::StatusCode code, const std::string& error_message = {}) override
    {
        if (_status == CREATE) {
            _status = PROCESS;
            waitForRequest();

        } else if (_status == PROCESS) {
            auto rp = createRequestProcessor();
            assert(rp != nullptr);
            rp->run(grpc::StatusCode::OK);

            if (code == grpc::StatusCode::OK)
                handleRequest();
            _status = FINISH;
            _responder.Finish(_reply, grpc::Status(code, error_message), this);

        } else {
            GPR_ASSERT(_status == FINISH);
            delete this;
        }
    }

    //! Контекст запроса
    grpc::ServerContext* context() const final { return &_context; }

protected:
    //! Очередь
    grpc::ServerCompletionQueue* queue() const { return _queue; }
    //! Обработчик потока
    const RequestWorker* worker() const { return _worker; }
    //! Сервис
    ServiceType* service() const { return _service; }    
    //! Запрос
    RequestType* request() const { return &_request; }
    //! Ответ
    ReplyType* reply() const { return &_reply; }
    //! Объект для отправки ответа
    grpc::ServerAsyncResponseWriter<ReplyType>* responder() const { return &_responder; }

    //! Фабрика по созданию обработчиков запроса
    virtual sl::RequestProcessor<ServiceType, RequestType, ReplyType>* createRequestProcessor() = 0;

private:        
    //! Очередь
    grpc::ServerCompletionQueue* _queue = nullptr;
    //! Контекст запроса
    mutable grpc::ServerContext _context;
    //! Обработчик потока
    const RequestWorker* _worker = nullptr;
    //! Сервис
    ServiceType* _service = nullptr;
    //! Запрос
    mutable RequestType _request;
    //! Ответ
    mutable ReplyType _reply;
    //! Объект для отправки ответа
    mutable grpc::ServerAsyncResponseWriter<ReplyType> _responder;

    enum CallStatus
    {
        CREATE,
        PROCESS,
        FINISH
    };
    CallStatus _status = CREATE;
};

} // namespace hrs
