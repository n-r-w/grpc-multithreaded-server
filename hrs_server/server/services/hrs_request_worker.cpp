#include "hrs_request_worker.h"
#include "hrs_factory.h"

#include <services/sql/srv_sql.h>
#include <services/test/srv_test.h>
#include <services/auth/srv_auth.h>
#include "hrs_auth.h"

namespace hrs
{
HrsRequestWorker::HrsRequestWorker(grpc::ServerCompletionQueue* queue, sl::UserValidator* user_validator)
    : RequestWorker(queue, user_validator)
{
}

std::vector<sl::RequestProcessorBase*> HrsRequestWorker::createRequestProcessors(const std::string& service_key) const
{
    if (service_key == HrsServiceFactory::AUTH_SERVICE_KEY)
        return {
            new LoginRequestProcessor(queue(), this),
            new KeepAliveRequestProcessor(queue(), this),
        };

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

sl::RequestWorker::UservValidationExtractResult HrsRequestWorker::extractUserValidationInfo(const grpc::Service* service,
                                                                                            const grpc::ServerContext* context,
                                                                                            std::string& session_id) const
{
    if (HrsServiceFactory::instance()->isAuthService(service))
        return UservValidationExtractResult::NotRequired;

    auto meta = context->client_metadata().find(UserValidator::SESSION_METADATA);
    if (meta != context->client_metadata().end())
        session_id = std::string(meta->second.begin(), meta->second.end());
    else
        return UservValidationExtractResult::NotFound;

    return UservValidationExtractResult::Extracted;
}

} // namespace hrs
