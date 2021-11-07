#pragma once
#include <string>
#include <map>
#include <vector>
#include <memory>

namespace sql
{
class BinderParser;

class Binder
{
public:
    Binder();
    Binder(
        //! SQL шаблон
        const std::string& sql_template);
    ~Binder();

    //! Есть ли уже связь между данной переменной и значением
    bool isBinded(const std::string& variable) const;
    //! Получено ли уже результирующее sql выражение
    bool isSqlCalculated() const;

    //! Задан ли шаблон
    bool isPrepared() const;
    //! Сбросить все бинды и задать новый шаблон
    void prepare(
        //! SQL шаблон
        const std::string& sql_template);

    //! Заменить в строке Sql бинд формата :bind на значение переменной value
    void bind(const std::string& variable, bool value);
    void bind(const std::string& variable, int16_t value);
    void bind(const std::string& variable, uint16_t value);
    void bind(const std::string& variable, int64_t value);
    void bind(const std::string& variable, uint64_t value);
    void bind(const std::string& variable, double value);
    void bind(const std::string& variable, float value);

    void bind(const std::string& variable, const std::string& value,
              //! Обрезать строки по краям
              bool trim_string = true,
              //! Закрывать строку ковычками
              bool format_string = true);
    void bind(const std::string& variable, const char* value,
              //! Обрезать строки по краям
              bool trim_string = true,
              //! Закрывать строку ковычками
              bool format_string = true);

    //! Очистить свзяь переменной со значением
    void unbind(const std::string& variable);

    //! Получить результат подстановки переменных в шаблон
    std::string sql(
        //! Критическая ошибка если есть не использованные, но инициализированные параметры
        bool halt_if_not_used = true) const;

    //! Очистить бинды для повторного вызова ZSqlBinder::sql с новыми значениями переменных
    void clearBinds();

    //! Список переменных в SQL выражении
    std::vector<std::string> parsedVariables() const;
    //! Проверяет есть ли такая переменная в списке распарсеных
    bool isVariableParsed(const std::string& variable) const;

    //*** Статические методы для замены только одной переменной и получения результата
    //! Заменить в строке Sql бинд формата :bind на значение переменной value
    static std::string bindOne(const std::string& sql_template, const std::string& variable, bool value);
    static std::string bindOne(const std::string& sql_template, const std::string& variable, int16_t value);
    static std::string bindOne(const std::string& sql_template, const std::string& variable, uint16_t value);
    static std::string bindOne(const std::string& sql_template, const std::string& variable, int64_t value);
    static std::string bindOne(const std::string& sql_template, const std::string& variable, uint64_t value);
    static std::string bindOne(const std::string& sql_template, const std::string& variable, double value);
    static std::string bindOne(const std::string& sql_template, const std::string& variable, float value);
    static std::string bindOne(const std::string& sql_template, const std::string& variable, const std::string& value,
                               //! Обрезать строки по краям
                               bool trim_string = true,
                               //! Закрывать строку ковычками
                               bool format_string = true);
    static std::string bindOne(const std::string& sql_template, const std::string& variable, const char* value,
                               //! Обрезать строки по краям
                               bool trim_string = true,
                               //! Закрывать строку ковычками
                               bool format_string = true);

private:
    void clear();
    BinderParser* data() const;

    mutable std::unique_ptr<BinderParser> _data;
    //! SQL текст
    std::string _sql_template;

    //! Связь переменных со значениями. Ключ - название переменной в нижнем регистре
    std::map<std::string, std::string> _values;
    //! Результат подстановки переменных
    mutable std::string _sql;
    //! Был ли выполнен расчет через sql
    mutable bool _is_calculated = false;
};

} // namespace sql
