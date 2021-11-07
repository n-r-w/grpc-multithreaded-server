#pragma once

#include <iostream>
#include <mutex>
#include <atomic>

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
    //! Генерация случайной строки на основе sha256
    static std::string generateId();

    //! Перевод строки в нижний регистр. Только для ASCII
    static std::string toLower(const std::string& s);
    //! Является ли символ алфовитно-цифровым (английская раскладка)
    static bool isAlnum(char ch);

    // trim from start (in place)
    static void ltrim(std::string& s);
    // trim from end (in place)
    static void rtrim(std::string& s);
    // trim from both ends (in place)
    static void trim(std::string& s);
    // trim from start (copying)
    static std::string ltrim_copy(std::string s);
    // trim from end (copying)
    static std::string rtrim_copy(std::string s);
    // trim from both ends (copying)
    static std::string trim_copy(std::string s);

private:
    //! блокировка параллельного вывода в консоль
    static std::mutex _cout_locker;

    static std::mutex _id_mutex;
    static uint64_t _id_base;
};

} // namespace hrs
