#pragma once

#include <server_lib/sl_server.h>
#include <api/generated/sql/sql.grpc.pb.h>
#include <services/hrs_factory.h>

namespace hrs
{
class HrsServer : public sl::Server
{
public:
    HrsServer();

protected:
    //! Фабрика по создание обработчиков очереди (отдельный поток)
    sl::RequestWorker* createWorker(grpc::ServerCompletionQueue* queue) override;

private:
    SqlApi::Sql::AsyncService _service;
    HrsServiceFactory _service_factory;
};
} // namespace hrs
