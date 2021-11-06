#pragma once

#include <server_lib/sl_server.h>
#include <server_lib/sl_request_worker.h>
#include <server_lib/sl_request_processor.h>

#include <api/generated/srv/srv.grpc.pb.h>
#include "../hrs_factory.h"

namespace hrs
{
//! Обработчик запроса Auth::checkLogin
SL_DEFINE_PROCESSOR(AuthRequestProcessor, Srv::Auth::AsyncService, HrsServiceFactory::AUTH_SERVICE_KEY, Srv::LoginRequest, Srv::LoginReply,
                    checkLogin)

} // namespace hrs
