#pragma once

#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>

namespace sl
{
//! Фабрика по созданию сервисов
class ServiceFactory
{
public:
    ServiceFactory(const std::vector<std::string>& service_keys);
    virtual ~ServiceFactory();

    //! Общедоступный указатель на фабрику
    static ServiceFactory* instance();

    //! Список идентификаторов зарегистрированных сервисов
    const std::vector<std::string>& serviceKeys() const;
    //! Получить сервис по его идентификатору
    grpc::Service* getService(const std::string& key) const;

protected:
    //! Создание сервиса по его идентификатору
    virtual grpc::Service* createService(const std::string& key) const = 0;

private:
    std::vector<std::string> _service_keys;
    mutable std::map<std::string, std::unique_ptr<grpc::Service>> _services;

    mutable std::mutex _mutex;
};
} // namespace sl
