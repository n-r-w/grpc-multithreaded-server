#include "sl_connection_pool.h"
#include "sl_error.h"
#include "sl_connection.h"
#include "sl_query.h"

#include <sl_utils.h>
#include <assert.h>
#include <chrono>

namespace sql
{
std::atomic<bool> ConnectionPool::_is_shutdown(false);

void ConnectionPool::shutdown()
{
    _is_shutdown = true;
}

ConnectionPool::ConnectionPool(size_t capacity)
    : _capacity(capacity)
{
    _salt = sl::Utils::createSalt();
}

ConnectionPool::~ConnectionPool()
{
    clear();
}

ConnectionPtr ConnectionPool::getConnection(const std::string& host, size_t port, const std::string& db_name, const std::string& login,
                                            const std::string& password, const std::string& options, sl::Error& error) const
{
    assert(!_is_shutdown);

    std::string password_hash = sl::Utils::sha256(_salt + password);
    std::string key = createKey(host, port, db_name, login, password_hash, options);

    Connection* connection = nullptr;

    _mutex.lock();
    auto i = _connections.find(key);
    if (i != _connections.end()) {
        connection = i->second->front();
        i->second->pop();

        if (i->second->size() == 0)
            _connections.erase(i);

        _mutex.unlock();

    } else {
        _mutex.unlock();
        connection
            = const_cast<ConnectionPool*>(this)->createConnection(host, port, db_name, login, password, password_hash, options, error);
        if (error.isError()) {
            assert(connection == nullptr);
            return nullptr;
        }

        assert(connection != nullptr);
        error = connection->error();
        if (error.isError()) {
            delete connection;
            return nullptr;
        }

        assert(connection->isOpen());
    }

    return std::shared_ptr<Connection>(connection,
                                       // нестандартный метод удаления объекта
                                       [this](Connection* c) {
                                           if (_is_shutdown)
                                               delete c;
                                           else
                                               freeConnection(c);
                                       });
}

size_t ConnectionPool::clear()
{
    std::lock_guard<std::mutex> lock(_mutex);

    size_t count = 0;
    for (auto q = _connections.cbegin(); q != _connections.cend(); ++q) {
        while (q->second->size() > 0) {
            delete q->second->front();
            q->second->pop();
            count++;
        }
    }

    return count;
}

sl::Error ConnectionPool::setConnectionOptions(const std::string& host, size_t port, const std::string& db_name, const std::string& login,
                                               const std::string& password, const std::string& options)
{
    clear();

    // создаем тестовое соединение. послу удаления оно уйдет в пул
    sl::Error error;
    auto c = getConnection(host, port, db_name, login, password, options, error);
    if (error.isError())
        return error;

    _host = host;
    _port = port;
    _db_name = db_name;
    _login = login;
    _password = password;
    _options = options;
    return {};
}

QueryPtr ConnectionPool::getQuery(sl::Error& error)
{
    auto c = getConnection(_host, _port, _db_name, _login, _password, _options, error);
    if (error.isError())
        return nullptr;

    auto q = createQuery(c);
    return std::shared_ptr<Query>(q);
}

void ConnectionPool::freeConnection(Connection* c) const
{
    assert(!_is_shutdown);

    if (!c->isOpen()) {
        delete c;
        return;
    }

    std::string key = createKey(c->host(), c->port(), c->database(), c->login(), c->passwordHash(), c->options());

    std::lock_guard<std::mutex> lock(_mutex);
    auto i = _connections.find(key);
    if (i != _connections.end()) {
        if (i->second->size() < _capacity)
            i->second->push(c);
        else
            delete c;

    } else {
        auto queue = std::make_unique<std::queue<Connection*>>();
        queue->push(c);
        _connections[key] = std::move(queue);
    }
}

std::string ConnectionPool::createKey(const std::string& host, size_t port, const std::string& db_name, const std::string& login,
                                      const std::string& password_hash, const std::string& options) const
{
    static const std::string sp = "_";
    return sl::Utils::sha256(_salt + host + sp + std::to_string(port) + sp + db_name + sp + login + sp + password_hash + sp + options);
}
} // namespace sql
