#pragma once

#include <memory>
#include <mutex>
#include <assert.h>
#include <variant>
#include <vector>

#include "sl_error.h"

namespace sql
{
class Connection
{
public:
    Connection();
    Connection(const std::string& options);
    virtual ~Connection();

    bool isValid() const;

    std::string_view options() const;

    bool isOpen() const;
    Error open();
    Error close();

protected:
    virtual bool impl_isOpen() const = 0;
    virtual Error impl_open() = 0;
    virtual Error impl_close() = 0;

    std::string _options;
    mutable std::mutex _mutex;
};
using ConnectionPtr = std::shared_ptr<Connection>;

class SqlDriver;
using SqlDriverPtr = std::shared_ptr<SqlDriver>;

class SqlValue
{
public:
    enum Type
    {
        Undefined,
        Bool, // -> bool
        LongInt, // -> int64_t
        Int, // -> int32_t
        Double, // -> double
        Date, // -> int64_t MSecsSinceEpoch
        Time, // -> int64_t MSecs
        DateTime, // ->int64_t MSecsSinceEpoch
        Bytes, // -> std::vector<std::byte>
        String, // -> std::string
    };

    SqlValue() { }

    Type type() const { return _type; }

protected:
private:
    Type _type = Undefined;
    std::variant<bool, int64_t, int32_t, double, std::vector<std::byte>, std::string> _value;
};

using SqlValuePtr = std::shared_ptr<SqlValue>;

class SqlField;
using SqlFieldPtr = std::shared_ptr<SqlField>;

class SqlRecord;
using SqlRecordPtr = std::shared_ptr<SqlRecord>;

class Query
{
public:
    Query() { }
    virtual ~Query() { }

    bool isValid() const { return _is_valid; }
    bool isSelect() const { return _is_select; }

    SqlDriverPtr driver() const { return _driver; }

protected:
    Query(const SqlDriverPtr& driver)
        : _driver(driver)
        , _is_valid(true)
    {
        assert(_driver != nullptr);
    }

    void setSelect(bool s) { _is_select = s; }

protected:
    virtual Error exec(const std::string& command) = 0;
    virtual bool next() = 0;
    virtual SqlRecordPtr record() = 0;

private:
    SqlDriverPtr _driver;
    bool _is_valid = false;
    bool _is_select = false;
};
using QueryPtr = std::shared_ptr<Query>;

} // namespace sql
