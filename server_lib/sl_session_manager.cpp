#include "sl_session_manager.h"
#include <sl_utils.h>
#include <vector>

namespace sl
{
SessionManager::SessionManager()
    : _expire_sec(120)
{
}

void SessionManager::setExpire(const std::chrono::seconds& expire_sec)
{
    _expire_sec = expire_sec;
}

std::chrono::seconds SessionManager::expire() const
{
    return _expire_sec;
}

std::string SessionManager::createNewSession(const std::string& login)
{
    std::string id = sl::Utils::generateId();

    std::lock_guard<std::mutex> lock(_mutex);
    removeSessionByLogin_helper(login);
    _sessions[id] = std::chrono::system_clock::now();
    return id;
}

bool SessionManager::checkSession(const std::string& session_id)
{
    std::lock_guard<std::mutex> lock(_mutex);
    return checkSession_helper(session_id);
}

bool SessionManager::removeSessionById(const std::string& session_id)
{
    std::lock_guard<std::mutex> lock(_mutex);
    return removeSessionById_helper(session_id);
}

bool SessionManager::removeSessionByLogin(const std::string& login)
{
    std::lock_guard<std::mutex> lock(_mutex);
    return removeSessionByLogin_helper(login);
}

void SessionManager::clearExpired()
{
    std::lock_guard<std::mutex> lock(_mutex);

    std::vector<std::string> to_check;

    for (auto i = _sessions.cbegin(); i != _sessions.cend(); ++i) {
        to_check.push_back(i->first);
    }

    for (auto& s : to_check) {
        checkSession_helper(s);
    }
}

bool SessionManager::removeSessionById_helper(const std::string& session_id)
{
    auto i = _sessions.find(session_id);
    if (i == _sessions.end())
        return false;

    for (auto l = _logins.cbegin(); l != _logins.cend(); ++l) {
        if (l->second != session_id)
            continue;

        _logins.erase(l);
        break;
    }

    return true;
}

bool SessionManager::removeSessionByLogin_helper(const std::string& login)
{
    auto i = _logins.find(login);
    if (i == _logins.end())
        return false;

    _sessions.erase(i->second);
    _logins.erase(i);
    return true;
}

bool SessionManager::checkSession_helper(const std::string& session_id)
{
    auto i = _sessions.find(session_id);
    if (i == _sessions.end())
        return false;

    auto diff = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - i->second);
    if (diff > _expire_sec) {
        removeSessionById_helper(session_id);
        return false;

    } else {
        i->second = std::chrono::system_clock::now();
        return true;
    }
}

} // namespace hrs
