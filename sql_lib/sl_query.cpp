#include "sl_query.h"

namespace sql
{
Query::~Query()
{
}

ConnectionPtr Query::connection() const
{
    return _con;
}

Query::ResultType Query::type() const
{
    return _type;
}

sl::Error Query::exec(const std::string& sql)
{
    assert(!sql.empty());
    sl::Error error = doExec(sql, _type);
    return error;
}

Query::Query(const ConnectionPtr& con)
    : _con(con)
{
}

} // namespace sql
