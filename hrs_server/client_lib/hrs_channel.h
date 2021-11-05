#pragma once

#include <grpcpp/channel.h>
#include <grpcpp/security/credentials.h>
#include <utils/sl_error.h>

namespace hrs
{
class ClientChannel
{
public:
    //! Соединение с сервером
    static sl::Error setup(const grpc::string& target, const std::shared_ptr<grpc::ChannelCredentials>& creds, const std::string& login,
                           const std::string& password);

    //! Канал с сервером
    static std::shared_ptr<grpc::Channel> instance();
    //! Создание нового контекста перед отправкой запроса
    static std::unique_ptr<grpc::ClientContext> createContext();

private:
    static std::mutex _mutex;
    static std::shared_ptr<grpc::Channel> _channel;
    static std::string _login;
    static std::string _password;
};
} // namespace hrs
