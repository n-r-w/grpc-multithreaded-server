#pragma once

#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>

namespace sl
{
//! Механизм проверки пользователей
class UserValidator
{
public:
    UserValidator();
    virtual ~UserValidator();

    bool checkUser(const std::string& login, const std::string& password);
    void clearUserInfo(const std::string& login);

protected:
    virtual bool getPassword(const std::string& login, std::string& password) = 0;

private:
    static std::string calcHash(const std::string& password);

    std::mutex _mutex;
    std::map<std::string, std::string> _info;
};

/* В теории это надо устанавливать как-то так. Но как его потом проверять на сервере совершенно непонятно
        auto call_creds = grpc::MetadataCredentialsFromPlugin(
            std::unique_ptr<grpc::MetadataCredentialsPlugin>(new sl::SimpleAuthenticator("my login", "my password")));
        context.set_credentials(call_creds);
*/
class SimpleAuthenticator : public grpc::MetadataCredentialsPlugin
{
public:
    SimpleAuthenticator(const grpc::string& login, const grpc::string& password);

    grpc::Status GetMetadata(grpc::string_ref service_url, grpc::string_ref method_name, const grpc::AuthContext& channel_auth_context,
                             std::multimap<grpc::string, grpc::string>* metadata) override;

private:
    grpc::string _login;
    grpc::string _password;
};

} // namespace sl
