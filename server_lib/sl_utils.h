#pragma once

#include <iostream>
#include <mutex>

namespace sl
{
class Utils
{
public:
    //! Потокобезопасный вывод в консоль
    static void coutPrint(const std::string& message);

private:
    //! блокировка параллельного вывода в консоль
    static std::mutex _cout_locker;
};

} // namespace hrs
