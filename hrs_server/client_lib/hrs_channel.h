#pragma once

#include <grpcpp/channel.h>
#include <grpcpp/security/credentials.h>
#include <utils/sl_error.h>

namespace hrs
{
class KeepAliveWorker;

//! Колбек - функция для информирования о состоянии канала
typedef std::function<void(sl::Error)> ClientChannelCallbackFunction;

class ClientChannel
{
public:
    //! Соединение с сервером
    static sl::Error connect(const grpc::string& target, const std::shared_ptr<grpc::ChannelCredentials>& creds, const std::string& login,
                             const std::string& password);
    //! Отключение от сервера
    static void disconnect();

    static void setCallback(ClientChannelCallbackFunction f);

    //! Канал с сервером
    static std::shared_ptr<grpc::Channel> instance();
    //! Создание нового контекста перед отправкой запроса
    static std::unique_ptr<grpc::ClientContext> createContext();

private:
    //! Поддержка соединения с сервером
    static sl::Error keepAlive();

    static std::mutex _mutex;
    static std::shared_ptr<grpc::Channel> _channel;
    static std::string _login;
    static std::string _password;
    static std::string _session_id;
    static std::unique_ptr<KeepAliveWorker> _keep_alive_worker;
    static ClientChannelCallbackFunction _callback;

    friend class KeepAliveWorker;
};
} // namespace hrs
