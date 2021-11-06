#include "hrs_auth.h"
#include <sl_utils.h>

namespace hrs
{
UserValidator::UserValidator()
    : sl::UserValidator()
{
}

UserValidator::~UserValidator()
{
}

bool UserValidator::getLoginPasswordHash(const std::string& login, std::string& password)
{
    password = calcPasswordHash("qq1234");
    return true;
}

std::string UserValidator::calcPasswordHash(const std::string& password_hash)
{
    return sl::Utils::sha256(password_hash);
}

} // namespace hrs
