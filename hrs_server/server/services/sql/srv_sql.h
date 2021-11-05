#pragma once

#include <server_lib/sl_server.h>
#include <server_lib/sl_request_worker.h>
#include <server_lib/sl_request_processor.h>

#include <api/generated/sql/sql.grpc.pb.h>
#include <api/generated/shared/shared.grpc.pb.h>
#include "../hrs_factory.h"

namespace hrs
{
//! Обработчик запроса Sql::ExecuteSQL
SL_DEFINE_PROCESSOR(SqlRequestProcessor, SqlApi::Sql::AsyncService, HrsServiceFactory::SQL_SERVICE_KEY, SqlApi::SqlRequest,
                    SqlApi::SqlReply, ExecuteSQL)

} // namespace impl
