#pragma once

#include <server_lib/sl_server.h>
#include <server_lib/sl_request_worker.h>
#include <server_lib/sl_request_processor.h>
#include <api/generated/test/test.grpc.pb.h>
#include "../hrs_factory.h"

namespace hrs
{
//! Обработчик запроса TestService::ExecuteTestRequest1
SL_DEFINE_PROCESSOR(TestRequestProcessor1, TestApi::TestService::AsyncService, HrsServiceFactory::TEST_SERVICE_KEY, TestApi::TestRequest1,
                    TestApi::TestReply1, ExecuteTestRequest1)

//! Обработчик запроса TestService::ExecuteTestRequest2
SL_DEFINE_PROCESSOR(TestRequestProcessor2, TestApi::TestService::AsyncService, HrsServiceFactory::TEST_SERVICE_KEY, TestApi::TestRequest2,
                    TestApi::TestReply2, ExecuteTestRequest2)

} // namespace hrs
