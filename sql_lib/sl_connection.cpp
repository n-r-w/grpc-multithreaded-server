#include "sl_connection.h"

namespace sql
{
Connection::Connection()
{
}

Connection::Connection(const std::string& options)
    : _options(options)
{
    assert(!_options.empty());
}

Connection::~Connection()
{
}

bool Connection::isValid() const
{
    return !_options.empty();
}

bool Connection::isOpen() const
{
    if (!isValid())
        return false;

    std::lock_guard<std::mutex> lock(_mutex);
    return impl_isOpen();
}

Error Connection::open()
{
    assert(isValid());
    std::lock_guard<std::mutex> lock(_mutex);
    return impl_open();
}

Error Connection::close()
{
    assert(isValid());
    std::lock_guard<std::mutex> lock(_mutex);
    return impl_close();
}

std::string_view Connection::options() const
{
    return _options;
}

} // namespace sql
