#pragma once

#include <server_lib/sl_server.h>
#include <server_lib/sl_request_worker.h>
#include <server_lib/sl_request_processor.h>
#include <api/generated/test/test.grpc.pb.h>
#include "../hrs_factory.h"

namespace hrs
{
//! Обработчик запроса TestService::ExecuteTestRequest1
SL_DEFINE_PROCESSOR(TestRequestProcessor1, TestApi::TestService::AsyncService, TestApi::TestRequest, TestApi::TestReply,
                    ExecuteTestRequest1)

//! Обработчик запроса TestService::ExecuteTestRequest2
SL_DEFINE_PROCESSOR(TestRequestProcessor2, TestApi::TestService::AsyncService, TestApi::TestRequest, TestApi::TestReply,
                    ExecuteTestRequest2)

} // namespace hrs
