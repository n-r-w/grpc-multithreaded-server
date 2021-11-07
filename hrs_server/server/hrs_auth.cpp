#include "hrs_auth.h"
#include <sl_utils.h>
#include "services/hrs_factory.h"
#include <boost/algorithm/string/replace.hpp>

namespace hrs
{
UserValidator::UserValidator()
    : sl::UserValidator()
{
}

UserValidator::~UserValidator()
{
}

bool UserValidator::getLoginPasswordHash(const std::string& login, std::string& password_hash, sl::Error& error)
{
    auto query = HrsServiceFactory::getQuery(error);

    if (error.isOk()) {
        std::string sql = "SELECT password_hash FROM users WHERE login=':login'";
        boost::replace_all(sql, ":login", login);
        error = query->exec(sql);
    }

    if (error.isError() || query->rowCount() == 0)
        return false;

    password_hash = query->toString(0, 0);
    return true;
}

std::string UserValidator::calcPasswordHash(const std::string& password)
{
    return sl::Utils::sha256(password);
}

} // namespace hrs
