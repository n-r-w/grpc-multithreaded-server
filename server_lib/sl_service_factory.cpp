#include "sl_service_factory.h"
#include <grpcpp/impl/codegen/service_type.h>
#include "sl_server.h"

namespace sl
{
ServiceFactory::ServiceFactory(const std::vector<std::string>& service_keys)
    : _service_keys(service_keys)
{
    assert(!_service_keys.empty());
}

ServiceFactory::~ServiceFactory()
{
}

ServiceFactory* ServiceFactory::instance()
{
    return Server::serviceFactory();
}

const std::vector<std::string>& ServiceFactory::serviceKeys() const
{
    return _service_keys;
}

grpc::Service* ServiceFactory::getService(const std::string& key) const
{
    assert(!key.empty());
    std::lock_guard<std::mutex> lock(_mutex);

    auto i = _services.find(key);
    if (i == _services.end()) {
        assert(std::find(_service_keys.cbegin(), _service_keys.cend(), key) != _service_keys.cend());
        auto s = createService(key);
        assert(s != nullptr);
        _services[key] = std::move(std::unique_ptr<grpc::Service>(s));
        i = _services.find(key);
    }

    return i->second.get();
}

} // namespace sl
