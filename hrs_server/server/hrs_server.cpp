#include "hrs_server.h"
#include <services/sql/srv_sql.h>
#include <services/hrs_request_worker.h>

namespace hrs
{
HrsServer::HrsServer(uint16_t expire_sec)
    : sl::Server(&_service_factory, &_user_validator)
{
    _user_validator.setExpire(std::chrono::seconds(expire_sec));
    _service_factory.setUserValidator(&_user_validator);
}

sl::Error HrsServer::initDatabaseConnection(const std::string& host, size_t port, const std::string& db_name, const std::string& login,
                                            const std::string& password, const std::string& options)
{
    return _service_factory.sqlConnectionPool()->setConnectionOptions(host, port, db_name, login, password, options);
}

sl::RequestWorker* HrsServer::createWorker(grpc::ServerCompletionQueue* queue)
{
    return new HrsRequestWorker(queue, &_user_validator);
}

} // namespace hrs
