#include "sl_error.h"

namespace sl
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
    _code = std::move(e._code);
    _text = std::move(e._text);
    return *this;
}

bool Error::operator==(const Error& e) const
{
    if ((_code == nullptr) != (e._code == nullptr))
        return false;
    return (_code == nullptr || *_code == *e._code);
}

bool Error::isError() const
{
    return _code != nullptr;
}

bool Error::isOk() const
{
    return _code == nullptr;
}

std::string Error::text() const
{
    return _text;
}

int64_t Error::code() const
{
    return _code != nullptr ? *_code : 0;
}

void Error::clear()
{
    *this = Error();
}

std::string Error::fullText() const
{
    if (_code == nullptr)
        return "no error";

    if (_text.empty())
        return "error code: " + std::to_string(*_code);

    return _text + " (error code: " + std::to_string(*_code) + ")";
}

} // namespace sql
