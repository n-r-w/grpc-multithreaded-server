#pragma once

#include "sl_stoppable_worker.h"
#include "sl_request_processor.h"
#include "sl_service_factory.h"

#include <grpcpp/grpcpp.h>
#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/health_check_service_interface.h>

namespace sl
{
//! Обработчик очереди, работает в отдельном потоке и отвечает за создание обработчиков запросов
class RequestWorker : public StoppableWorker
{
public:
    RequestWorker(
        //! Очередь
        grpc::ServerCompletionQueue* queue);
    virtual ~RequestWorker();

    void start();

    void stopRequest();
    void stopWait();

    void runRequestProcessorsPool() const;

protected:
    grpc::ServerCompletionQueue* queue() const;

    //! Фабрика по созданию обработчиков запросов
    virtual std::vector<sl::RequestProcessorBase*> createRequestProcessors(const std::string& service_key) const = 0;

private:
    void process();

    //! Очередь
    grpc::ServerCompletionQueue* _queue = nullptr;

    // Поток, в котором запускается
    std::thread* _thread = nullptr;
    bool _stopped = false;
    bool _is_shutdowned = false;
};

} // namespace hrs