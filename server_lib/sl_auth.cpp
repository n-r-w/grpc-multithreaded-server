#include "sl_auth.h"

namespace sl
{
SimpleAuthenticator::SimpleAuthenticator(const std::string& login, const std::string& password)
    : _login(login)
    , _password(password)
{
}

grpc::Status SimpleAuthenticator::GetMetadata(grpc::string_ref service_url, grpc::string_ref method_name,
                                              const grpc::AuthContext& channel_auth_context,
                                              std::multimap<std::string, std::string>* metadata)
{
    metadata->insert(std::make_pair("x-custom-auth-login", _login));
    metadata->insert(std::make_pair("x-custom-auth-password", _password));
    return grpc::Status::OK;
}

UserValidator::UserValidator()
{
}

UserValidator::~UserValidator()
{
}

bool UserValidator::checkUser(const std::string& login, const std::string& password)
{
    std::lock_guard<std::mutex> lock(_mutex);

    std::string request_hash = calcHash(password);

    auto i = _info.find(login);
    if (i != _info.end())
        return request_hash == i->second;

    std::string pass;
    if (!getPassword(login, pass))
        return false;

    std::string valid_hash = calcHash(pass);
    _info[login] = valid_hash;

    return valid_hash == request_hash;
}

void UserValidator::clearUserInfo(const std::string& login)
{
    std::lock_guard<std::mutex> lock(_mutex);
    _info.erase(login);
}

std::string UserValidator::calcHash(const std::string& password)
{
    return password;
}

} // namespace sl
