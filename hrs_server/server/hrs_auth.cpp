#include "hrs_auth.h"

namespace hrs
{
UserValidator::UserValidator()
{
}

UserValidator::~UserValidator()
{
}

bool UserValidator::getPassword(const std::string& login, std::string& password)
{
    password = "qq1234";
    return true;
}

} // namespace hrs
