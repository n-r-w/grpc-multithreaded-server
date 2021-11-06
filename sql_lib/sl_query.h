#pragma once

#include "sl_error.h"
#include <memory>

namespace sql
{
class Query;
using QueryPtr = std::shared_ptr<Query>;

class Connection;
using ConnectionPtr = std::shared_ptr<Connection>;

//! Запрос к БД
class Query
{
public:
    virtual ~Query();

    //! Тип запроса
    enum class ResultType
    {
        Undefined,
        //! Ошибка
        Error,
        //! Запрос данных
        Select,
        //! Команда, которая не подразумевает получение результата
        Command,
    };

    //! Соединение
    ConnectionPtr connection() const;
    //! Тип запроса. Инициализируется после выполнения запроса
    ResultType type() const;
    //! Выполнить запрос
    sl::Error exec(const std::string& sql);

    //! Количество строк в результате
    virtual size_t rowCount() const = 0;
    //! Количество колонок в результате
    virtual size_t columnCount() const = 0;

    //! Преобразование значения в строку
    virtual std::string toString(size_t row, size_t column) const = 0;

protected:
    //! Выполнение запроса
    virtual sl::Error doExec(const std::string& sql, ResultType& result_type) = 0;

    Query(const ConnectionPtr& con);

private:
    ConnectionPtr _con;
    ResultType _type = ResultType::Undefined;

    friend class ConnectionPool;
};

} // namespace sql
