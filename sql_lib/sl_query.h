#pragma once

#include "sl_error.h"
#include <memory>

namespace sql
{
class Query;
using QueryPtr = std::shared_ptr<Query>;

class Connection;
using ConnectionPtr = std::shared_ptr<Connection>;

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

    ConnectionPtr connection() const;
    ResultType type() const;

    Error exec(const std::string& sql);

    virtual size_t rowCount() const = 0;
    virtual size_t columnCount() const = 0;

    virtual std::string toString(size_t row, size_t column) const = 0;

protected:
    virtual Error doExec(const std::string& sql, ResultType& result_type) = 0;

    Query(const ConnectionPtr& con);

private:
    ConnectionPtr _con;
    ResultType _type = ResultType::Undefined;
};

} // namespace sql
