#pragma once

#include <memory>
#include <mutex>
#include <assert.h>
#include <variant>
#include <vector>
#include <map>
#include <queue>

#include "sl_error.h"

namespace sql
{
class Connection;
using ConnectionPtr = std::shared_ptr<Connection>;

//! Соединение с сервером
class Connection
{
public:
    virtual ~Connection();

    std::string host() const;
    size_t port() const;
    std::string database() const;
    std::string login() const;
    std::string passwordHash() const;

    std::string options() const;

    virtual bool isOpen() const = 0;
    virtual sl::Error error() const = 0;

protected:
    Connection(const std::string& host, size_t port, const std::string& db_name, const std::string& login, const std::string& password,
               const std::string& password_hash, const std::string& options);

private:
    std::string _host;
    size_t _port;
    std::string _login;
    std::string _password_hash;
    std::string _options;
    std::string _db_name;

    friend class ConnectionPool;
};

} // namespace sql
