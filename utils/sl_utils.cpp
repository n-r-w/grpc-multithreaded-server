#include "sl_utils.h"
#include <algorithm>

namespace sl
{
//! блокировка параллельного вывода в консоль
std::mutex Utils::_cout_locker;

void Utils::coutPrint(const std::string& message)
{
    std::lock_guard<std::mutex> lock(_cout_locker);
    std::cout << message << std::endl;
}

std::string Utils::toLower(const std::string& s)
{
    std::string res;
    res.resize(s.size());
    std::transform(s.begin(), s.end(), res.begin(), ::tolower);
    return res;
}

bool Utils::isAlnum(char c)
{
    return (c >= 0x30 && c <= 0x39) || (c >= 0x41 && c <= 0x5A) || (c >= 0x61 && c <= 0x7A) || c == 0x5F;
}

void Utils::ltrim(std::string& s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) { return !std::isspace(ch); }));
}

void Utils::rtrim(std::string& s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) { return !std::isspace(ch); }).base(), s.end());
}

void Utils::trim(std::string& s)
{
    ltrim(s);
    rtrim(s);
}

std::string Utils::ltrim_copy(std::string s)
{
    ltrim(s);
    return s;
}

std::string Utils::rtrim_copy(std::string s)
{
    rtrim(s);
    return s;
}

std::string Utils::trim_copy(std::string s)
{
    trim(s);
    return s;
}

} // namespace hrs
