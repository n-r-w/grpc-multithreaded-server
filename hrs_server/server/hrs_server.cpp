#include "hrs_server.h"
#include <services/sql/srv_sql.h>
#include <services/hrs_request_worker.h>

namespace hrs
{
HrsServer::HrsServer()
    : sl::Server(&_service_factory, &_user_validator)
{
}

sl::RequestWorker* HrsServer::createWorker(grpc::ServerCompletionQueue* queue)
{
    return new HrsRequestWorker(queue, &_user_validator);
}
} // namespace hrs
