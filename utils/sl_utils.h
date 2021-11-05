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
    //! Получить хэш sha256
    static std::string sha256(const std::string& input);
    //! Создать перфикс для sha256 на основе текущего времени
    static std::string createSalt();

private:
    //! блокировка параллельного вывода в консоль
    static std::mutex _cout_locker;
};

} // namespace hrs
