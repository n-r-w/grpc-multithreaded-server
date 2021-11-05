#include "hrs_channel.h"
#include <grpcpp/grpcpp.h>
#include <api/src/hrs_error_codes.h>
#include <hrs_server/server/hrs_auth.h>
#include <api/generated/shared/shared.grpc.pb.h>

namespace hrs
{
std::mutex ClientChannel::_mutex;
std::shared_ptr<grpc::Channel> ClientChannel::_channel;
std::string ClientChannel::_login;
std::string ClientChannel::_password;

sl::Error ClientChannel::setup(const std::string& target, const std::shared_ptr<grpc::ChannelCredentials>& creds, const std::string& login,
                               const std::string& password)
{
    std::lock_guard<std::mutex> lock(_mutex);

    _channel = grpc::CreateChannel(target, creds);
    auto state = _channel->GetState(true);
    if (state != GRPC_CHANNEL_READY && state != GRPC_CHANNEL_IDLE) {
        _channel.reset();
        return sl::Error(ErrorCodes::ConnectionFail);
    }

    ProtoShared::LoginRequest request;
    request.set_login(login);
    request.set_password(password);

    ProtoShared::LoginReply reply;
    auto stub = ProtoShared::Auth::NewStub(_channel);

    grpc::ClientContext context;

    auto login_status = stub->checkLogin(&context, request, &reply);
    if (!login_status.ok() || reply.error_code() != 0) {
        _channel.reset();
        return sl::Error(ErrorCodes::AuthFail);
    }

    _login = login;
    _password = password;

    return {};
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

    context->AddMetadata(hrs::UserValidator::LOGIN_METADATA, _login);
    context->AddMetadata(hrs::UserValidator::PASSWORD_METADATA, _password);

    return std::move(context);
}

} // namespace hrs
