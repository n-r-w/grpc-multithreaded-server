#include "sl_auth.h"
#include "sl_session_manager.h"

namespace sl
{
UserValidator::UserValidator()
    : _session_manager(std::make_unique<SessionManager>())
{
}

UserValidator::~UserValidator()
{
}

void UserValidator::setExpire(const std::chrono::seconds& expire_sec)
{
    _session_manager->setExpire(expire_sec);
}

std::chrono::seconds UserValidator::expire() const
{
    return _session_manager->expire();
}

bool UserValidator::checkSession(const std::string& session_id)
{
    std::lock_guard<std::mutex> lock(_mutex);
    return _session_manager->checkSession(session_id);
}

void UserValidator::clearLogin(const std::string& login)
{
    std::lock_guard<std::mutex> lock(_mutex);
    _session_manager->removeSessionByLogin(login);
}

void UserValidator::clearSession(const std::string& session_id)
{
    std::lock_guard<std::mutex> lock(_mutex);
    _session_manager->removeSessionById(session_id);
}

bool UserValidator::login(const std::string& login, const std::string& password, std::string& session_id, sl::Error& error)
{
    std::lock_guard<std::mutex> lock(_mutex);

    session_id.erase();
    std::string request_hash = calcPasswordHash(password);
    std::string valid_hash;
    if (!getLoginPasswordHash(login, valid_hash, error))
        return false;

    if (valid_hash != request_hash)
        return false;

    session_id = _session_manager->createNewSession(login);
    return true;
}

} // namespace sl
