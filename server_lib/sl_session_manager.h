#pragma once
#include <memory>
#include <mutex>
#include <map>
#include <chrono>

namespace sl
{
//! Управление сессиями пользователей
class SessionManager
{
public:
    SessionManager();

    //! Время устаревания сессии без подтверждения активности (секунды)
    void setExpire(const std::chrono::seconds& expire_sec);
    //! Время устаревания сессии без подтверждения активности
    std::chrono::seconds expire() const;

    //! Создать новую сессию для логина
    std::string createNewSession(const std::string& login);
    //! Проверка сессии на активность. Сбрасывает таймер устаревания
    bool checkSession(const std::string& session_id);
    //! Удалении сессии по ID
    bool removeSessionById(const std::string& session_id);
    //! Удалении сессии по логину
    bool removeSessionByLogin(const std::string& login);

    //! Очистить информацию о просроченных сессиях
    void clearExpired();

private:
    bool removeSessionById_helper(const std::string& session_id);
    bool removeSessionByLogin_helper(const std::string& login);
    bool checkSession_helper(const std::string& session_id);

    std::mutex _mutex;
    //! Ключ - id сессии, значение
    std::map<std::string, std::chrono::time_point<std::chrono::system_clock>> _sessions;
    //! Ключ - логин, значение - сессия
    std::map<std::string, std::string> _logins;
    //! Время устаревания сессии без подтверждения активности
    std::chrono::seconds _expire_sec;
};

} // namespace hrs
