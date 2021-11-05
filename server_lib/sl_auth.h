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
    virtual bool getPassword(const std::string& login, std::string& password) = 0;

private:
    static std::string calcHash(const std::string& password);

    std::mutex _mutex;
    std::map<std::string, std::string> _info;
};

} // namespace sl
