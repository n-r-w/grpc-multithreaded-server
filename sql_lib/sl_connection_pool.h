#pragma once

#include <memory>
#include <atomic>
#include <mutex>
#include <map>
#include <queue>

#include <sl_error.h>

namespace sql
{
class Connection;
using ConnectionPtr = std::shared_ptr<Connection>;

class Query;
using QueryPtr = std::shared_ptr<Query>;

//! Пул соединений к БД. Фабрика соединений и запросов
class ConnectionPool
{
public:    
    ConnectionPool(
        //! Максимальный размер пула для каждого типа соединений (комбинации адреса, логина и прочего)
        size_t capacity);
    virtual ~ConnectionPool();

    //! Завершить работу с соединениями. Освобожденные соединения перестанут попадать в пул
    static void shutdown();

    //! Очистить пул. Потокобезопасно. Возвращает количество удаленных соединений
    size_t clear();

    //! Задать параметры подключения. Вызывать до того, как начнется работа с запросами. Не потокобезопасно
    sl::Error setConnectionOptions(const std::string& host, size_t port, const std::string& db_name, const std::string& login,
                                   const std::string& password, const std::string& options);

    //! Создать запрос. Потокобезопасно
    QueryPtr getQuery(sl::Error& error);

    //    virtual std::string valueToSql(...) = 0;

protected:
    //! Создать соединение. Реализация зависит от конкретного SQL сервера
    virtual Connection* createConnection(const std::string& host, size_t port, const std::string& db_name, const std::string& login,
                                         const std::string& password, const std::string& password_hash, const std::string& options,
                                         sl::Error& error)
        = 0;

    //! Создать запрос. Реализация зависит от конкретного SQL сервера
    virtual Query* createQuery(const ConnectionPtr& connection) = 0;

private:
    //! Получить соединение из пула или создать при необходимости новое. При окончании использования оно само попадет в пул
    ConnectionPtr getConnection(const std::string& host, size_t port, const std::string& db_name, const std::string& login,
                                const std::string& password, const std::string& options, sl::Error& error) const;
    //! Освободить соединение
    void freeConnection(Connection* c) const;
    //! Уникальный ключ типа соединения
    std::string createKey(const std::string& host, size_t port, const std::string& db_name, const std::string& login,
                          const std::string& password_hash, const std::string& options) const;

    mutable std::mutex _mutex;
    std::string _salt;
    size_t _capacity;
    mutable std::map<std::string, std::unique_ptr<std::queue<Connection*>>> _connections;

    std::string _host;
    size_t _port;
    std::string _db_name;
    std::string _login;
    std::string _password;
    std::string _options;

    static std::atomic<bool> _is_shutdown;
};
} // namespace sql
