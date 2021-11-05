// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: shared.proto
#ifndef GRPC_shared_2eproto__INCLUDED
#define GRPC_shared_2eproto__INCLUDED

#include "shared.pb.h"

#include <functional>
#include <grpcpp/impl/codegen/async_generic_service.h>
#include <grpcpp/impl/codegen/async_stream.h>
#include <grpcpp/impl/codegen/async_unary_call.h>
#include <grpcpp/impl/codegen/client_callback.h>
#include <grpcpp/impl/codegen/client_context.h>
#include <grpcpp/impl/codegen/completion_queue.h>
#include <grpcpp/impl/codegen/message_allocator.h>
#include <grpcpp/impl/codegen/method_handler.h>
#include <grpcpp/impl/codegen/proto_utils.h>
#include <grpcpp/impl/codegen/rpc_method.h>
#include <grpcpp/impl/codegen/server_callback.h>
#include <grpcpp/impl/codegen/server_callback_handlers.h>
#include <grpcpp/impl/codegen/server_context.h>
#include <grpcpp/impl/codegen/service_type.h>
#include <grpcpp/impl/codegen/status.h>
#include <grpcpp/impl/codegen/stub_options.h>
#include <grpcpp/impl/codegen/sync_stream.h>

namespace ProtoShared {

class Auth final {
 public:
  static constexpr char const* service_full_name() {
    return "ProtoShared.Auth";
  }
  class StubInterface {
   public:
    virtual ~StubInterface() {}
    // Проверка логина и пароля
    virtual ::grpc::Status checkLogin(::grpc::ClientContext* context, const ::ProtoShared::LoginRequest& request, ::ProtoShared::LoginReply* response) = 0;
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::ProtoShared::LoginReply>> AsynccheckLogin(::grpc::ClientContext* context, const ::ProtoShared::LoginRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::ProtoShared::LoginReply>>(AsynccheckLoginRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::ProtoShared::LoginReply>> PrepareAsynccheckLogin(::grpc::ClientContext* context, const ::ProtoShared::LoginRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::ProtoShared::LoginReply>>(PrepareAsynccheckLoginRaw(context, request, cq));
    }
    class async_interface {
     public:
      virtual ~async_interface() {}
      // Проверка логина и пароля
      virtual void checkLogin(::grpc::ClientContext* context, const ::ProtoShared::LoginRequest* request, ::ProtoShared::LoginReply* response, std::function<void(::grpc::Status)>) = 0;
      virtual void checkLogin(::grpc::ClientContext* context, const ::ProtoShared::LoginRequest* request, ::ProtoShared::LoginReply* response, ::grpc::ClientUnaryReactor* reactor) = 0;
    };
    typedef class async_interface experimental_async_interface;
    virtual class async_interface* async() { return nullptr; }
    class async_interface* experimental_async() { return async(); }
   private:
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::ProtoShared::LoginReply>* AsynccheckLoginRaw(::grpc::ClientContext* context, const ::ProtoShared::LoginRequest& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::ProtoShared::LoginReply>* PrepareAsynccheckLoginRaw(::grpc::ClientContext* context, const ::ProtoShared::LoginRequest& request, ::grpc::CompletionQueue* cq) = 0;
  };
  class Stub final : public StubInterface {
   public:
    Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options = ::grpc::StubOptions());
    ::grpc::Status checkLogin(::grpc::ClientContext* context, const ::ProtoShared::LoginRequest& request, ::ProtoShared::LoginReply* response) override;
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::ProtoShared::LoginReply>> AsynccheckLogin(::grpc::ClientContext* context, const ::ProtoShared::LoginRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::ProtoShared::LoginReply>>(AsynccheckLoginRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::ProtoShared::LoginReply>> PrepareAsynccheckLogin(::grpc::ClientContext* context, const ::ProtoShared::LoginRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::ProtoShared::LoginReply>>(PrepareAsynccheckLoginRaw(context, request, cq));
    }
    class async final :
      public StubInterface::async_interface {
     public:
      void checkLogin(::grpc::ClientContext* context, const ::ProtoShared::LoginRequest* request, ::ProtoShared::LoginReply* response, std::function<void(::grpc::Status)>) override;
      void checkLogin(::grpc::ClientContext* context, const ::ProtoShared::LoginRequest* request, ::ProtoShared::LoginReply* response, ::grpc::ClientUnaryReactor* reactor) override;
     private:
      friend class Stub;
      explicit async(Stub* stub): stub_(stub) { }
      Stub* stub() { return stub_; }
      Stub* stub_;
    };
    class async* async() override { return &async_stub_; }

   private:
    std::shared_ptr< ::grpc::ChannelInterface> channel_;
    class async async_stub_{this};
    ::grpc::ClientAsyncResponseReader< ::ProtoShared::LoginReply>* AsynccheckLoginRaw(::grpc::ClientContext* context, const ::ProtoShared::LoginRequest& request, ::grpc::CompletionQueue* cq) override;
    ::grpc::ClientAsyncResponseReader< ::ProtoShared::LoginReply>* PrepareAsynccheckLoginRaw(::grpc::ClientContext* context, const ::ProtoShared::LoginRequest& request, ::grpc::CompletionQueue* cq) override;
    const ::grpc::internal::RpcMethod rpcmethod_checkLogin_;
  };
  static std::unique_ptr<Stub> NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options = ::grpc::StubOptions());

  class Service : public ::grpc::Service {
   public:
    Service();
    virtual ~Service();
    // Проверка логина и пароля
    virtual ::grpc::Status checkLogin(::grpc::ServerContext* context, const ::ProtoShared::LoginRequest* request, ::ProtoShared::LoginReply* response);
  };
  template <class BaseClass>
  class WithAsyncMethod_checkLogin : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithAsyncMethod_checkLogin() {
      ::grpc::Service::MarkMethodAsync(0);
    }
    ~WithAsyncMethod_checkLogin() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status checkLogin(::grpc::ServerContext* /*context*/, const ::ProtoShared::LoginRequest* /*request*/, ::ProtoShared::LoginReply* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestcheckLogin(::grpc::ServerContext* context, ::ProtoShared::LoginRequest* request, ::grpc::ServerAsyncResponseWriter< ::ProtoShared::LoginReply>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(0, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  typedef WithAsyncMethod_checkLogin<Service > AsyncService;
  template <class BaseClass>
  class WithCallbackMethod_checkLogin : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithCallbackMethod_checkLogin() {
      ::grpc::Service::MarkMethodCallback(0,
          new ::grpc::internal::CallbackUnaryHandler< ::ProtoShared::LoginRequest, ::ProtoShared::LoginReply>(
            [this](
                   ::grpc::CallbackServerContext* context, const ::ProtoShared::LoginRequest* request, ::ProtoShared::LoginReply* response) { return this->checkLogin(context, request, response); }));}
    void SetMessageAllocatorFor_checkLogin(
        ::grpc::MessageAllocator< ::ProtoShared::LoginRequest, ::ProtoShared::LoginReply>* allocator) {
      ::grpc::internal::MethodHandler* const handler = ::grpc::Service::GetHandler(0);
      static_cast<::grpc::internal::CallbackUnaryHandler< ::ProtoShared::LoginRequest, ::ProtoShared::LoginReply>*>(handler)
              ->SetMessageAllocator(allocator);
    }
    ~WithCallbackMethod_checkLogin() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status checkLogin(::grpc::ServerContext* /*context*/, const ::ProtoShared::LoginRequest* /*request*/, ::ProtoShared::LoginReply* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    virtual ::grpc::ServerUnaryReactor* checkLogin(
      ::grpc::CallbackServerContext* /*context*/, const ::ProtoShared::LoginRequest* /*request*/, ::ProtoShared::LoginReply* /*response*/)  { return nullptr; }
  };
  typedef WithCallbackMethod_checkLogin<Service > CallbackService;
  typedef CallbackService ExperimentalCallbackService;
  template <class BaseClass>
  class WithGenericMethod_checkLogin : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithGenericMethod_checkLogin() {
      ::grpc::Service::MarkMethodGeneric(0);
    }
    ~WithGenericMethod_checkLogin() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status checkLogin(::grpc::ServerContext* /*context*/, const ::ProtoShared::LoginRequest* /*request*/, ::ProtoShared::LoginReply* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  template <class BaseClass>
  class WithRawMethod_checkLogin : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithRawMethod_checkLogin() {
      ::grpc::Service::MarkMethodRaw(0);
    }
    ~WithRawMethod_checkLogin() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status checkLogin(::grpc::ServerContext* /*context*/, const ::ProtoShared::LoginRequest* /*request*/, ::ProtoShared::LoginReply* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestcheckLogin(::grpc::ServerContext* context, ::grpc::ByteBuffer* request, ::grpc::ServerAsyncResponseWriter< ::grpc::ByteBuffer>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(0, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class WithRawCallbackMethod_checkLogin : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithRawCallbackMethod_checkLogin() {
      ::grpc::Service::MarkMethodRawCallback(0,
          new ::grpc::internal::CallbackUnaryHandler< ::grpc::ByteBuffer, ::grpc::ByteBuffer>(
            [this](
                   ::grpc::CallbackServerContext* context, const ::grpc::ByteBuffer* request, ::grpc::ByteBuffer* response) { return this->checkLogin(context, request, response); }));
    }
    ~WithRawCallbackMethod_checkLogin() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status checkLogin(::grpc::ServerContext* /*context*/, const ::ProtoShared::LoginRequest* /*request*/, ::ProtoShared::LoginReply* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    virtual ::grpc::ServerUnaryReactor* checkLogin(
      ::grpc::CallbackServerContext* /*context*/, const ::grpc::ByteBuffer* /*request*/, ::grpc::ByteBuffer* /*response*/)  { return nullptr; }
  };
  template <class BaseClass>
  class WithStreamedUnaryMethod_checkLogin : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithStreamedUnaryMethod_checkLogin() {
      ::grpc::Service::MarkMethodStreamed(0,
        new ::grpc::internal::StreamedUnaryHandler<
          ::ProtoShared::LoginRequest, ::ProtoShared::LoginReply>(
            [this](::grpc::ServerContext* context,
                   ::grpc::ServerUnaryStreamer<
                     ::ProtoShared::LoginRequest, ::ProtoShared::LoginReply>* streamer) {
                       return this->StreamedcheckLogin(context,
                         streamer);
                  }));
    }
    ~WithStreamedUnaryMethod_checkLogin() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable regular version of this method
    ::grpc::Status checkLogin(::grpc::ServerContext* /*context*/, const ::ProtoShared::LoginRequest* /*request*/, ::ProtoShared::LoginReply* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    // replace default version of method with streamed unary
    virtual ::grpc::Status StreamedcheckLogin(::grpc::ServerContext* context, ::grpc::ServerUnaryStreamer< ::ProtoShared::LoginRequest,::ProtoShared::LoginReply>* server_unary_streamer) = 0;
  };
  typedef WithStreamedUnaryMethod_checkLogin<Service > StreamedUnaryService;
  typedef Service SplitStreamedService;
  typedef WithStreamedUnaryMethod_checkLogin<Service > StreamedService;
};

}  // namespace ProtoShared


#endif  // GRPC_shared_2eproto__INCLUDED
