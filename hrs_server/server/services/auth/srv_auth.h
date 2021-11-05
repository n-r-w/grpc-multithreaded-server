#pragma once

#include <server_lib/sl_server.h>
#include <server_lib/sl_request_worker.h>
#include <server_lib/sl_request_processor.h>

#include <api/generated/shared/shared.grpc.pb.h>
#include "../hrs_factory.h"

namespace hrs
{
//! Обработчик запроса Auth::checkLogin
SL_DEFINE_PROCESSOR(AuthRequestProcessor, ProtoShared::Auth::AsyncService, HrsServiceFactory::AUTH_SERVICE_KEY, ProtoShared::LoginRequest,
                    ProtoShared::LoginReply, checkLogin)

} // namespace hrs
