#pragma once

#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>

#include "sl_request_worker.h"
#include "sl_service_factory.h"

namespace sl
{
class Server : protected StoppableWorker
{
public:
    Server(ServiceFactory* service_factory);
    virtual ~Server();

    static ServiceFactory* serviceFactory();

    void start(const std::string& server_address, size_t thread_count);
    void stop();

protected:
    //! Создание обработчика очереди. Будет работать в отдельном потоке
    virtual sl::RequestWorker* createWorker(grpc::ServerCompletionQueue* queue) = 0;

private:    
    void process();

    std::string _server_address;
    size_t _thread_count = 0;

    std::unique_ptr<grpc::ServerBuilder> _builder;    
    std::unique_ptr<grpc::Server> _server;
    ServiceFactory* _service_factory = nullptr;

    std::unique_ptr<grpc::ServerCompletionQueue> _queue;
    std::vector<RequestWorker*> _workers;

    // Поток, в котором запускается
    std::thread* _thread = nullptr;

    static ServiceFactory* _service_factory_global;
};

} // namespace hrs
