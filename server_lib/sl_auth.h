#pragma once

#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>

namespace sl
{
//! Механизм проверки пользователей
class UserValidator
{
public:
    UserValidator();
    virtual ~UserValidator();

    bool checkUser(const std::string& login, const std::string& password);
    void clearUserInfo(const std::string& login);

protected:
    //! Метод должен вернуть хэш акутального пароля для данного логина
    virtual bool getLoginPasswordHash(const std::string& login, std::string& password_hash) = 0;
    //! Метод должен вычислить хэш пароля
    virtual std::string calcPasswordHash(const std::string& password_hash) = 0;

private:
    std::mutex _mutex;
    std::map<std::string, std::string> _info;
};

} // namespace sl
