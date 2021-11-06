#pragma once

#include <server_lib/sl_server.h>
#include <api/generated/sql/sql.grpc.pb.h>
#include <services/hrs_factory.h>
#include "hrs_auth.h"

namespace hrs
{
class HrsServer : public sl::Server
{
public:
    HrsServer();

    //! Инициализация соединения с БД
    sl::Error initDatabaseConnection(const std::string& host, size_t port, const std::string& db_name, const std::string& login,
                                     const std::string& password, const std::string& options);

protected:
    //! Фабрика по создание обработчиков очереди (отдельный поток)
    sl::RequestWorker* createWorker(grpc::ServerCompletionQueue* queue) override;

private:
    SqlApi::Sql::AsyncService _service;
    HrsServiceFactory _service_factory;
    UserValidator _user_validator;
};
} // namespace hrs
