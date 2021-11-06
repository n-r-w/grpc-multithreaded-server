#include "psql_impl.h"
#include <libpq-fe.h>

namespace sql
{
PsqlConnectionPool::PsqlConnectionPool(size_t capacity)
    : ConnectionPool(capacity)
{
}

PsqlConnectionPool::~PsqlConnectionPool()
{
}

Connection* PsqlConnectionPool::createConnection(const std::string& host, size_t port, const std::string& db_name, const std::string& login,
                                                 const std::string& password, const std::string& password_hash, const std::string& options,
                                                 sl::Error& error)
{
    return new PsqlConnection(host, port, db_name, login, password, password_hash, options);
}

Query* PsqlConnectionPool::createQuery(const ConnectionPtr& connection)
{
    return new PsqlQuery(connection);
}

PsqlConnection::~PsqlConnection()
{
    PQfinish((PGconn*)_pq_connection);
}

bool PsqlConnection::isOpen() const
{
    return PQstatus((PGconn*)_pq_connection) == CONNECTION_OK;
}

sl::Error PsqlConnection::error() const
{
    auto status = PQstatus((PGconn*)_pq_connection);
    if (status != CONNECTION_OK)
        return sl::Error(status, PQerrorMessage((PGconn*)_pq_connection));

    return {};
}

PsqlConnection::PsqlConnection(const std::string& host, size_t port, const std::string& db_name, const std::string& login,
                               const std::string& password, const std::string& password_hash, const std::string& options)
    : Connection(host, port, db_name, login, password, password_hash, options)
{
    std::string c_string;
    c_string = "host=" + host + " port=" + std::to_string(port) + " dbname=" + db_name + " user=" + login + " password=" + password;
    if (!options.empty())
        c_string += " " + options;

    _pq_connection = PQconnectdb(c_string.c_str());
}

PsqlQuery::PsqlQuery(const ConnectionPtr& con)
    : Query(con)
{
}

PsqlQuery::~PsqlQuery()
{
    if (_result != nullptr)
        PQclear((PGresult*)_result);
}

PsqlConnectionPtr PsqlQuery::connection() const
{
    return std::dynamic_pointer_cast<PsqlConnection>(Query::connection());
}

size_t PsqlQuery::rowCount() const
{
    return _result != nullptr ? PQntuples((PGresult*)_result) : 0;
}

size_t PsqlQuery::columnCount() const
{
    return _result != nullptr ? PQnfields((PGresult*)_result) : 0;
}

std::string PsqlQuery::toString(size_t row, size_t column) const
{
    assert(_result != nullptr);
    const char* res = PQgetvalue((PGresult*)_result, row, column);
    return std::string(res);
}

sl::Error PsqlQuery::doExec(const std::string& sql, ResultType& result_type)
{
    if (_result != nullptr)
        PQclear((PGresult*)_result);

    _result = PQexec((PGconn*)connection()->_pq_connection, sql.c_str());
    ExecStatusType status = PQresultStatus((PGresult*)_result);

    switch (status) {
        case PGRES_EMPTY_QUERY:
            result_type = ResultType::Error;
            return sl::Error(-1, "empty query");

        case PGRES_TUPLES_OK:
            result_type = ResultType::Select;
            return {};

        case PGRES_COMMAND_OK:
            result_type = ResultType::Command;
            return {};

        case PGRES_FATAL_ERROR:
        case PGRES_NONFATAL_ERROR:
            result_type = ResultType::Error;
            return sl::Error(status, PQerrorMessage((PGconn*)connection()->_pq_connection));

        default:
            result_type = ResultType::Error;
            return sl::Error(status, "unsupported query type");
    }
}

} // namespace sql
