#include "hrs_sql_request.h"
#include "hrs_channel.h"

namespace hrs
{
std::mutex SqlRequest::_mutex;
std::unique_ptr<SqlApi::Sql::Stub> SqlRequest::_stub;

SqlRequestPtr SqlRequest::exec(const std::string& sql, sl::Error& error)
{
    SqlApi::SqlRequest request;
    request.set_sql(sql);
    auto reply = std::make_shared<SqlApi::SqlReply>();

    auto context = ClientChannel::createContext();
    auto status = stub()->ExecuteSQL(context.get(), request, reply.get());
    if (!status.ok()) {
        error = sl::Error(ErrorCodes::RpcFail, status.error_message());
        return nullptr;
    }

    error.clear();
    return SqlRequestPtr(new SqlRequest(reply));
}

sql::Query::ResultType SqlRequest::type() const
{
    return static_cast<sql::Query::ResultType>(_reply->result_type());
}

const api::DatasetWrapper* SqlRequest::data() const
{
    assert(type() == sql::Query::ResultType::Select);
    return _wrapper->datasetReader(1);
}

SqlRequest::SqlRequest(const std::shared_ptr<SqlApi::SqlReply>& reply)
    : _reply(reply)
{
    _wrapper = api::DataContainerWrapper::createReader(_reply->data());
}

SqlApi::Sql::Stub* SqlRequest::stub()
{
    std::lock_guard<std::mutex> lock(_mutex);
    if (_stub == nullptr)
        _stub = SqlApi::Sql::NewStub(ClientChannel::instance());
    return _stub.get();
}
} // namespace hrs
