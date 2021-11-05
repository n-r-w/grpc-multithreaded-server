#include "sl_connection.h"
#include <sl_utils.h>
#include "sl_connection_pool.h"

namespace sql
{
Connection::~Connection()
{
}

std::string Connection::host() const
{
    return _host;
}

size_t Connection::port() const
{
    return _port;
}

std::string Connection::database() const
{
    return _db_name;
}

std::string Connection::login() const
{
    return _login;
}

std::string Connection::passwordHash() const
{
    return _password_hash;
}

std::string Connection::options() const
{
    return _options;
}

Connection::Connection(const std::string& host, size_t port, const std::string& db_name, const std::string& login,
                       const std::string& password, const std::string& password_hash, const std::string& options)
    : _host(host)
    , _port(port)
    , _login(login)
    , _db_name(db_name)
    , _password_hash(password_hash)
    , _options(options)
{
    assert(!_host.empty());
    assert(_port > 0);
    assert(!_login.empty());
    assert(!_db_name.empty());
    assert(!_password_hash.empty());
}

} // namespace sql
