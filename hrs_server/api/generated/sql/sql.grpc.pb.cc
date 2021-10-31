// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: sql.proto

#include "sql.pb.h"
#include "sql.grpc.pb.h"

#include <functional>
#include <grpcpp/impl/codegen/async_stream.h>
#include <grpcpp/impl/codegen/async_unary_call.h>
#include <grpcpp/impl/codegen/channel_interface.h>
#include <grpcpp/impl/codegen/client_unary_call.h>
#include <grpcpp/impl/codegen/client_callback.h>
#include <grpcpp/impl/codegen/message_allocator.h>
#include <grpcpp/impl/codegen/method_handler.h>
#include <grpcpp/impl/codegen/rpc_service_method.h>
#include <grpcpp/impl/codegen/server_callback.h>
#include <grpcpp/impl/codegen/server_callback_handlers.h>
#include <grpcpp/impl/codegen/server_context.h>
#include <grpcpp/impl/codegen/service_type.h>
#include <grpcpp/impl/codegen/sync_stream.h>
namespace SqlApi {

static const char* Sql_method_names[] = {
  "/SqlApi.Sql/ExecuteSQL",
};

std::unique_ptr< Sql::Stub> Sql::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  (void)options;
  std::unique_ptr< Sql::Stub> stub(new Sql::Stub(channel, options));
  return stub;
}

Sql::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options)
  : channel_(channel), rpcmethod_ExecuteSQL_(Sql_method_names[0], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  {}

::grpc::Status Sql::Stub::ExecuteSQL(::grpc::ClientContext* context, const ::SqlApi::SqlRequest& request, ::SqlApi::SqlReply* response) {
  return ::grpc::internal::BlockingUnaryCall< ::SqlApi::SqlRequest, ::SqlApi::SqlReply, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_ExecuteSQL_, context, request, response);
}

void Sql::Stub::async::ExecuteSQL(::grpc::ClientContext* context, const ::SqlApi::SqlRequest* request, ::SqlApi::SqlReply* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::SqlApi::SqlRequest, ::SqlApi::SqlReply, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_ExecuteSQL_, context, request, response, std::move(f));
}

void Sql::Stub::async::ExecuteSQL(::grpc::ClientContext* context, const ::SqlApi::SqlRequest* request, ::SqlApi::SqlReply* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_ExecuteSQL_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::SqlApi::SqlReply>* Sql::Stub::PrepareAsyncExecuteSQLRaw(::grpc::ClientContext* context, const ::SqlApi::SqlRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::SqlApi::SqlReply, ::SqlApi::SqlRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_ExecuteSQL_, context, request);
}

::grpc::ClientAsyncResponseReader< ::SqlApi::SqlReply>* Sql::Stub::AsyncExecuteSQLRaw(::grpc::ClientContext* context, const ::SqlApi::SqlRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncExecuteSQLRaw(context, request, cq);
  result->StartCall();
  return result;
}

Sql::Service::Service() {
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      Sql_method_names[0],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< Sql::Service, ::SqlApi::SqlRequest, ::SqlApi::SqlReply, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](Sql::Service* service,
             ::grpc::ServerContext* ctx,
             const ::SqlApi::SqlRequest* req,
             ::SqlApi::SqlReply* resp) {
               return service->ExecuteSQL(ctx, req, resp);
             }, this)));
}

Sql::Service::~Service() {
}

::grpc::Status Sql::Service::ExecuteSQL(::grpc::ServerContext* context, const ::SqlApi::SqlRequest* request, ::SqlApi::SqlReply* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


}  // namespace SqlApi

