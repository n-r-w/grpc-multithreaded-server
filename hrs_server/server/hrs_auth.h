#pragma once

#include <server_lib/sl_auth.h>

namespace hrs
{
//! Механизм проверки пользователей
class UserValidator : public sl::UserValidator
{
public:
    inline static const std::string LOGIN_METADATA = "hrs_login";
    inline static const std::string PASSWORD_METADATA = "hrs_password";

    UserValidator();
    ~UserValidator();

protected:
    virtual bool getPassword(const std::string& login, std::string& password) override;
};
} // namespace hrs
