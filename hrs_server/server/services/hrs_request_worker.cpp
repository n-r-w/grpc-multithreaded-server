#include "hrs_request_worker.h"
#include "hrs_factory.h"

#include <services/sql/srv_sql.h>
#include <services/test/srv_test.h>

namespace hrs
{
HrsRequestWorker::HrsRequestWorker(grpc::ServerCompletionQueue* queue)
    : RequestWorker(queue)
{
}

std::vector<sl::RequestProcessorBase*> HrsRequestWorker::createRequestProcessors(const std::string& service_key) const
{
    if (service_key == HrsServiceFactory::SQL_SERVICE_KEY)
        return {
            new SqlRequestProcessor(queue(), this),
        };

    if (service_key == HrsServiceFactory::TEST_SERVICE_KEY)
        return {
            new TestRequestProcessor1(queue(), this),
            new TestRequestProcessor2(queue(), this),
        };

    assert(false);
    return {};
}

} // namespace hrs
