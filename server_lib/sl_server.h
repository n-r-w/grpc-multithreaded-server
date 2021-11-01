#pragma once

#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>

#include "sl_request_worker.h"
#include "sl_service_factory.h"
#include "sl_auth.h"

namespace sl
{
class Server : protected StoppableWorker
{
public:
    Server(ServiceFactory* service_factory, UserValidator* user_validator);
    virtual ~Server();

    static ServiceFactory* serviceFactory();
    static UserValidator* userValidator();

    void start(const std::string& server_address, size_t thread_count, const std::string& root_cert = {}, const std::string& ssl_cert = {},
               const std::string& ssl_key = {});
    void stop();

protected:
    //! Создание обработчика очереди. Будет работать в отдельном потоке
    virtual sl::RequestWorker* createWorker(grpc::ServerCompletionQueue* queue) = 0;

private:    
    void process();

    std::string _server_address;
    size_t _thread_count = 0;

    std::string _root_cert;
    std::string _ssl_cert;
    std::string _ssl_key;

    std::unique_ptr<grpc::ServerBuilder> _builder;    
    std::unique_ptr<grpc::Server> _server;
    ServiceFactory* _service_factory = nullptr;
    UserValidator* _user_validator = nullptr;

    std::unique_ptr<grpc::ServerCompletionQueue> _queue;
    std::vector<RequestWorker*> _workers;

    // Поток, в котором запускается
    std::thread* _thread = nullptr;

    static ServiceFactory* _service_factory_global;
    static UserValidator* _user_validator_global;
};

} // namespace hrs
