#pragma once
#include <memory>
#include <string>
#include <string_view>
#include <assert.h>

namespace sql
{
class Error
{
public:
    Error() = default;
    Error(int64_t code, const std::string& text);
    Error(int64_t code, std::string&& text);
    Error(const Error& e);
    Error(Error&& e);
    Error& operator=(const Error& e);
    Error& operator=(Error&& e);

    //! Сравнение только по коду, текст игнорируется
    bool operator==(const Error& e) const;

    bool isError() const;
    bool isOk() const;

    std::string text() const;
    int64_t code() const;

private:
    std::unique_ptr<int64_t> _code;
    std::string _text;
};

} // namespace sql
