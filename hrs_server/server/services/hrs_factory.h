#pragma once

#include <server_lib/sl_service_factory.h>
#include <server_lib/sl_request_worker.h>
#include <sql_lib/sl_connection_pool.h>
#include <server_lib/sl_auth.h>

namespace hrs
{
//! Фабрика по созданию сервисов
class HrsServiceFactory : public sl::ServiceFactory
{
public:
    HrsServiceFactory();
    ~HrsServiceFactory();

    void setUserValidator(sl::UserValidator* v);

public:
    // Идентификаторы сервисов
    inline static const std::string AUTH_SERVICE_KEY = "auth";
    inline static const std::string SQL_SERVICE_KEY = "sql";
    inline static const std::string TEST_SERVICE_KEY = "test";

    //! Общедоступный указатель на фабрику
    static HrsServiceFactory* instance();

    //! Получение соединения с севрером БД для выполнения SQL запросов
    sql::ConnectionPool* sqlConnectionPool() const;
    //! Проверка пользователей
    sl::UserValidator* userValidator() const;

    //! Является ли сервсиом проверки логина и пароля
    bool isAuthService(const grpc::Service* service) const;

protected:
    //! Создание сервиса по его идентификатору
    grpc::Service* createService(const std::string& key) const override;

private:
    static std::vector<std::string> getServiceKeys();

    std::unique_ptr<sql::ConnectionPool> _conn_pool;
    sl::UserValidator* _user_validator = nullptr;
    mutable grpc::Service* _auth_service = nullptr;
};

} // namespace hrs
