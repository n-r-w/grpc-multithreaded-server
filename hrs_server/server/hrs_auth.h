#pragma once

#include <server_lib/sl_auth.h>

namespace hrs
{
//! Механизм проверки пользователей
class UserValidator : public sl::UserValidator
{
public:
    inline static const std::string SESSION_METADATA = "hrs_session_id";

    UserValidator();
    ~UserValidator();

protected:
    //! Метод должен вернуть хэш акутального пароля для данного логина
    bool getLoginPasswordHash(const std::string& login, std::string& password_hash) override;
    //! Метод должен вычислить хэш пароля
    std::string calcPasswordHash(const std::string& password_hash) override;
};
} // namespace hrs
