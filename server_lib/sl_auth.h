#pragma once

#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>

#include <sl_error.h>

namespace sl
{
class SessionManager;

//! Механизм проверки пользователей
class UserValidator
{
public:
    UserValidator();
    virtual ~UserValidator();

    //! Время устаревания сессии без подтверждения активности (секунды)
    void setExpire(const std::chrono::seconds& expire_sec);
    //! Время устаревания сессии без подтверждения активности
    std::chrono::seconds expire() const;

    //! Проверка входа пользователя. Если все ОК, то возвращает идентификатор сессии
    bool login(const std::string& login, const std::string& password,
               //! Идентификатор сессии
               std::string& session_id, sl::Error& error);
    //! Проверка сессии на актуальность
    bool checkSession(const std::string& session_id);

    //! Очистка сессии пользователя
    void clearLogin(const std::string& login);
    //! Очистка сессии пользователя
    void clearSession(const std::string& session_id);

protected:
    //! Метод должен вернуть хэш акутального пароля для данного логина, если такой существует
    virtual bool getLoginPasswordHash(const std::string& login, std::string& password_hash, sl::Error& error) = 0;
    //! Метод должен вычислить хэш пароля
    virtual std::string calcPasswordHash(const std::string& password) = 0;

private:
    std::mutex _mutex;
    std::unique_ptr<SessionManager> _session_manager;
};

} // namespace sl
