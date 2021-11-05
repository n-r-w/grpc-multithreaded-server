#include "sl_auth.h"

namespace sl
{
UserValidator::UserValidator()
{
}

UserValidator::~UserValidator()
{
}

bool UserValidator::checkUser(const std::string& login, const std::string& password)
{
    std::lock_guard<std::mutex> lock(_mutex);

    std::string request_hash = calcPasswordHash(password);

    auto i = _info.find(login);
    if (i != _info.end())
        return request_hash == i->second;

    std::string valid_hash;
    if (!getLoginPasswordHash(login, valid_hash))
        return false;

    _info[login] = valid_hash;

    return valid_hash == request_hash;
}

void UserValidator::clearUserInfo(const std::string& login)
{
    std::lock_guard<std::mutex> lock(_mutex);
    _info.erase(login);
}


} // namespace sl
