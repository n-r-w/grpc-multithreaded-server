#pragma once

#include <memory>
#include <utils/sl_error.h>
#include <api/generated/sql/sql.grpc.pb.h>
#include <api/src/hrs_error_codes.h>
#include <api/src/hrs_data_converter.h>
#include <sql_lib/sl_query.h>

namespace hrs
{
class SqlRequest;
using SqlRequestPtr = std::shared_ptr<SqlRequest>;

class SqlRequest
{
public:
    static SqlRequestPtr exec(const std::string& sql, sl::Error& error);

    //! Тип запроса
    sql::Query::ResultType type() const;
    //! Данные. Только для sql::Query::ResultType::Select
    const api::DatasetWrapper* data() const;

private:
    SqlRequest(const std::shared_ptr<SqlApi::SqlReply>& reply);

    std::shared_ptr<SqlApi::SqlReply> _reply;
    api::DataContainerWrapperPtr _wrapper;

    static SqlApi::Sql::Stub* stub();
    static std::mutex _mutex;
    static std::unique_ptr<SqlApi::Sql::Stub> _stub;
};

} // namespace cl
