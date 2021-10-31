#include "hrs_server.h"
#include <services/sql/srv_sql.h>
#include <services/hrs_request_worker.h>

namespace hrs
{
HrsServer::HrsServer()
    : sl::Server(&_service_factory)
{
}

sl::RequestWorker* HrsServer::createWorker(grpc::ServerCompletionQueue* queue)
{
    return new HrsRequestWorker(queue);
}
} // namespace hrs
