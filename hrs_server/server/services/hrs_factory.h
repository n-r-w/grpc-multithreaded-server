#pragma once

#include <server_lib/sl_service_factory.h>
#include <server_lib/sl_request_worker.h>

namespace hrs
{
//! Фабрика по созданию сервисов
class HrsServiceFactory : public sl::ServiceFactory
{
public:
    HrsServiceFactory();

public:
    // Идентификаторы сервисов
    inline static const std::string SQL_SERVICE_KEY = "sql";
    inline static const std::string TEST_SERVICE_KEY = "test";

protected:
    //! Создание сервиса по его идентификатору
    grpc::Service* createService(const std::string& key) const override;

private:
    static std::vector<std::string> getServiceKeys();
};

} // namespace hrs
