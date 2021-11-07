#include "hrs_channel.h"
#include <grpcpp/grpcpp.h>
#include <api/src/hrs_error_codes.h>
#include <hrs_server/server/hrs_auth.h>
#include <api/generated/srv/srv.grpc.pb.h>
#include <utils/sl_stoppable_worker.h>
#include "hrs_keep_alive.h"

namespace hrs
{
std::mutex ClientChannel::_mutex;
std::shared_ptr<grpc::Channel> ClientChannel::_channel;
std::string ClientChannel::_login;
std::string ClientChannel::_password;
std::string ClientChannel::_session_id;
std::unique_ptr<KeepAliveWorker> ClientChannel::_keep_alive_worker;
ClientChannelCallbackFunction ClientChannel::_callback;

sl::Error ClientChannel::connect(const std::string& target, const std::shared_ptr<grpc::ChannelCredentials>& creds,
                                 const std::string& login, const std::string& password)
{
    std::lock_guard<std::mutex> lock(_mutex);

    //    grpc::ChannelArguments args;
    //    args.SetInt(GRPC_ARG_KEEPALIVE_TIME_MS, kKeepAliveTimeMs);
    //    args.SetInt(GRPC_ARG_KEEPALIVE_TIMEOUT_MS, kKeepAliveTimeoutMs);
    //    args.SetInt(GRPC_ARG_KEEPALIVE_PERMIT_WITHOUT_CALLS, true);
    //    args.SetInt(GRPC_ARG_HTTP2_MAX_PINGS_WITHOUT_DATA, 0);

    _channel = grpc::CreateChannel(target, creds);
    auto state = _channel->GetState(true);
    if (state != GRPC_CHANNEL_READY && state != GRPC_CHANNEL_IDLE) {
        _channel.reset();
        return sl::Error(ErrorCodes::ConnectionFail);
    }

    Srv::LoginRequest request;
    request.set_login(login);
    request.set_password(password);

    Srv::LoginReply reply;
    auto stub = Srv::Auth::NewStub(_channel);

    grpc::ClientContext context;

    auto login_status = stub->Login(&context, request, &reply);
    if (!login_status.ok() || reply.error_code() != 0) {
        _channel.reset();
        return sl::Error(ErrorCodes::AuthFail);
    }

    _login = login;
    _password = password;
    _session_id = reply.session_id();

    _keep_alive_worker = std::make_unique<KeepAliveWorker>(
        // уменьшаем интервал поддержки соединения, чтобы не влияли сетевые задержки
        std::chrono::seconds(std::max(reply.keep_alive_sec() / 2, reply.keep_alive_sec() - 20)));
    _keep_alive_worker->start();

    return {};
}

void ClientChannel::disconnect()
{
    std::lock_guard<std::mutex> lock(_mutex);

    _keep_alive_worker.reset();
    _channel.reset();
}

void ClientChannel::setCallback(ClientChannelCallbackFunction f)
{
    std::lock_guard<std::mutex> lock(_mutex);
    _callback = f;
}

std::shared_ptr<grpc::Channel> ClientChannel::instance()
{
    std::lock_guard<std::mutex> lock(_mutex);
    assert(_channel != nullptr);
    return _channel;
}

std::unique_ptr<grpc::ClientContext> ClientChannel::createContext()
{
    std::lock_guard<std::mutex> lock(_mutex);
    assert(_channel != nullptr);

    auto context = std::make_unique<grpc::ClientContext>();

    context->AddMetadata(hrs::UserValidator::SESSION_METADATA, _session_id);

    return std::move(context);
}

sl::Error ClientChannel::keepAlive()
{
    std::lock_guard<std::mutex> lock(_mutex);
    assert(_channel != nullptr);

    Srv::KeepAliveRequest request;
    request.set_session_id(_session_id);

    Srv::KeepAliveReply reply;
    auto stub = Srv::Auth::NewStub(_channel);

    grpc::ClientContext context;

    auto login_status = stub->KeepAlive(&context, request, &reply);
    if (!login_status.ok() || reply.error_code() != 0) {
        auto error = sl::Error(ErrorCodes::ConnectionFail);
        if (_callback)
            _callback(error);

        return error;
    }

    return {};
}

} // namespace hrs
