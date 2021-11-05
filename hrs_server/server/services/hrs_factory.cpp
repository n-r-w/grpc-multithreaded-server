#include "hrs_factory.h"

#include <api/generated/sql/sql.grpc.pb.h>
#include <api/generated/test/test.grpc.pb.h>
#include <api/generated/shared/shared.grpc.pb.h>

#include <sql_lib/plugins/psql/psql_impl.h>

namespace hrs
{
HrsServiceFactory::HrsServiceFactory()
    : sl::ServiceFactory(getServiceKeys())
    , _conn_pool(std::make_unique<sql::PsqlConnectionPool>(10))
{
}

HrsServiceFactory::~HrsServiceFactory()
{
    _conn_pool->shutdown();
}

void HrsServiceFactory::setUserValidator(sl::UserValidator* v)
{
    assert(v != nullptr);
    _user_validator = v;
}

HrsServiceFactory* HrsServiceFactory::instance()
{
    return static_cast<HrsServiceFactory*>(sl::ServiceFactory::instance());
}

sql::ConnectionPool* HrsServiceFactory::sqlConnectionPool() const
{
    return _conn_pool.get();
}

sl::UserValidator* HrsServiceFactory::userValidator() const
{
    return _user_validator;
}

bool HrsServiceFactory::isAuthService(const grpc::Service* service) const
{
    assert(_auth_service != nullptr);
    return _auth_service == service;
}

grpc::Service* HrsServiceFactory::createService(const std::string& key) const
{
    if (key == AUTH_SERVICE_KEY) {
        _auth_service = new ProtoShared::Auth::AsyncService;
        return _auth_service;
    }
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
        AUTH_SERVICE_KEY,
    };
}

} // namespace hrs
