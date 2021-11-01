#include "hrs_request_worker.h"
#include "hrs_factory.h"

#include <services/sql/srv_sql.h>
#include <services/test/srv_test.h>
#include "hrs_auth.h"

namespace hrs
{
HrsRequestWorker::HrsRequestWorker(grpc::ServerCompletionQueue* queue, sl::UserValidator* user_validator)
    : RequestWorker(queue, user_validator)
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

bool HrsRequestWorker::extractUserValidationInfo(const grpc::ServerContext* context, std::string& login, std::string& password) const
{
    auto meta = context->client_metadata().find(UserValidator::LOGIN_METADATA);
    if (meta != context->client_metadata().end())
        login = std::string(meta->second.begin(), meta->second.end());
    else
        return false;

    meta = context->client_metadata().find(UserValidator::PASSWORD_METADATA);
    if (meta != context->client_metadata().end())
        password = std::string(meta->second.begin(), meta->second.end());
    else
        return false;

    return true;
}

} // namespace hrs
