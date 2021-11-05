#pragma once

#include "../../sl_connection_pool.h"
#include "../../sl_connection.h"
#include "../../sl_query.h"

namespace sql
{
class PsqlConnectionPool : public ConnectionPool
{
public:
    PsqlConnectionPool(
        //! Максимальный размер пула для каждого типа соединений (комбинации адреса, логина и прочего)
        size_t capacity);
    ~PsqlConnectionPool();

protected:
    //! Создать соединение. Должно быть открыто
    Connection* createConnection(const std::string& host, size_t port, const std::string& db_name, const std::string& login,
                                 const std::string& password, const std::string& password_hash, const std::string& options,
                                 Error& error) override;
};

class PsqlConnection;
using PsqlConnectionPtr = std::shared_ptr<PsqlConnection>;

class PsqlConnection : public Connection
{
public:
    ~PsqlConnection();

    bool isOpen() const override;
    Error error() const override;

private:
    PsqlConnection(const std::string& host, size_t port, const std::string& db_name, const std::string& login, const std::string& password,
                   const std::string& password_hash, const std::string& options = {});

    friend class PsqlConnectionPool;
    friend class PsqlQuery;

    void* _pq_connection = nullptr;
};

class PsqlQuery : public Query
{
public:
    PsqlQuery(const ConnectionPtr& con);
    ~PsqlQuery();

    PsqlConnectionPtr connection() const;

    size_t rowCount() const override;
    size_t columnCount() const override;

    std::string toString(size_t row, size_t column) const override;

protected:
    Error doExec(const std::string& sql, ResultType& result_type) override;

private:
    void* _result = nullptr;
};

} // namespace sql
