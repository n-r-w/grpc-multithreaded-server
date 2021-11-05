#pragma once

#include <memory>
#include <atomic>
#include <mutex>
#include <map>
#include <queue>

namespace sql
{
class Error;
class Connection;
using ConnectionPtr = std::shared_ptr<Connection>;

class ConnectionPool
{
public:
    //! Завершить работу с соединениями. Освобожденные соединения перестанут попадать в пул
    static void shutdown();

    ConnectionPool(
        //! Максимальный размер пула для каждого типа соединений (комбинации адреса, логина и прочего)
        size_t capacity);
    virtual ~ConnectionPool();

    //! Создать соединение. При окончании использования оно само попадет в пул
    ConnectionPtr getConnection(const std::string& host, size_t port, const std::string& db_name, const std::string& login,
                                const std::string& password, const std::string& options, Error& error) const;
    //! Создать соединение. При окончании использования оно само попадет в пул
    template <class T>
    std::shared_ptr<T> getConnection(const std::string& host, size_t port, const std::string& db_name, const std::string& login,
                                     const std::string& password, const std::string& options, Error& error) const
    {
        return std::dynamic_pointer_cast<T>(getConnection(host, port, db_name, login, password, options, error));
    }

    //! Очистить пул. Возвращает количество удаленных соединений
    size_t clear();

protected:
    //! Создать соединение
    virtual Connection* createConnection(const std::string& host, size_t port, const std::string& db_name, const std::string& login,
                                         const std::string& password, const std::string& password_hash, const std::string& options,
                                         Error& error)
        = 0;

private:
    //! Освободить соединение
    void freeConnection(Connection* c) const;
    //! Уникальный ключ типа соединения
    std::string createKey(const std::string& host, size_t port, const std::string& db_name, const std::string& login,
                          const std::string& password_hash, const std::string& options) const;

    mutable std::mutex _mutex;
    std::string _salt;
    size_t _capacity;
    mutable std::map<std::string, std::unique_ptr<std::queue<Connection*>>> _connections;

    static std::atomic<bool> _is_shutdown;
};
} // namespace sql
