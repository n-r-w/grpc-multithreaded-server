#include "sl_error.h"

namespace sql
{
Error::Error(const Error& e)
    : _code(e._code != nullptr ? std::make_unique<int64_t>(*e._code) : nullptr)
    , _text(e.text())
{
}

Error::Error(int64_t code, const std::string& text)
    : _code(std::make_unique<int64_t>(code))
    , _text(text)
{
}

Error::Error(int64_t code, std::string&& text)
    : _code(std::make_unique<int64_t>(code))
    , _text(std::move(text))
{
}

Error::Error(Error&& e)
    : _code(std::move(e._code))
    , _text(std::move(e._text))
{
}

Error& Error::operator=(const Error& e)
{
    _code = (e._code != nullptr ? std::make_unique<int64_t>(*e._code) : nullptr);
    _text = e.text();
    return *this;
}

Error& Error::operator=(Error&& e)
{
    _code = std::make_unique<int64_t>(e._code);
    _text = std::move(e._text);
    return *this;
}

bool Error::operator==(const Error& e) const
{
    if ((_code == nullptr) != (e._code == nullptr))
        return false;
    return (_code == nullptr || *_code == *e._code);
}

bool Error::isValid() const
{
    return _code != nullptr;
}

std::string_view Error::text() const
{
    return _text;
}

int64_t Error::code() const
{
    return _code != nullptr ? *_code : 0;
}

} // namespace sql
