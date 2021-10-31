#include "hrs_factory.h"

#include <api/generated/sql/sql.grpc.pb.h>
#include <api/generated/test/test.grpc.pb.h>

namespace hrs
{
HrsServiceFactory::HrsServiceFactory()
    : sl::ServiceFactory(getServiceKeys())
{
}

grpc::Service* HrsServiceFactory::createService(const std::string& key) const
{
    if (key == SQL_SERVICE_KEY)
        return new SqlApi::Sql::AsyncService;
    if (key == TEST_SERVICE_KEY)
        return new TestApi::TestService::AsyncService;

    assert(false);
    return nullptr;
}

std::vector<std::string> HrsServiceFactory::getServiceKeys()
{
    return {
        TEST_SERVICE_KEY,
        SQL_SERVICE_KEY,
    };
}

} // namespace hrs
