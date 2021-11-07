#include "sl_binder.h"
#include <assert.h>
#include <sl_utils.h>

namespace sql
{
//! Парсер для выявления переменных вида :переменная в sql запросе
class BinderParser
{
public:
    BinderParser(const std::string& sql_template);
    ~BinderParser();

    //! Подставить значения в переменные и получить результат
    std::string calculate(const std::map<std::string, std::string>& values,
                          bool halt_if_not_used); //! Ключ - название переменной в нижнем регистре

    //! SQL шаблон
    const std::string& sql_template() const;

    //! Список переменных в SQL выражении
    std::vector<std::string> parsedVariables() const;
    //! Распармена ли переменная
    bool isVariableParsed(const std::string& variable) const;

private:
    //! Найти в шаблоне переменные вида :переменная
    void parse();

    //! Результаты парсинга для одной переменной
    struct Data
    {
        //! Название переменной в нижнем регистре
        std::string name;
        //! Положение переменной в строке sql
        int position;
    };

    //! SQL шаблон
    std::string _sql_template;
    //! Результаты парсинга
    std::vector<std::shared_ptr<Data>> _parsed;
    //!Ключ имя распарсенной переменной в нижнем регистре
    std::map<std::string, std::shared_ptr<Data>> _parsed_hash;
};

BinderParser::BinderParser(const std::string& sql_template)
    : _sql_template(sql_template)
{
    parse();
}

BinderParser::~BinderParser()
{
}

std::string BinderParser::calculate(const std::map<std::string, std::string>& values, bool halt_if_not_used)
{
    if (_parsed.empty())
        return _sql_template;

    std::string sql;

    int shift = 0;
    for (auto& d : _parsed) {
        // Остаток слева
        sql += _sql_template.substr(shift, d->position - shift);

        // Заменяем переменную
        auto i = values.find(d->name);
        if (i != values.end())
            sql += i->second;
        shift = d->position + d->name.length();
    }

    // Остаток справа
    int right_pos = _sql_template.size() - (_parsed.back()->position + _parsed.back()->name.size());
    return sql + _sql_template.substr(_sql_template.size() - right_pos, right_pos);
}

const std::string& BinderParser::sql_template() const
{
    return _sql_template;
}

std::vector<std::string> BinderParser::parsedVariables() const
{
    std::vector<std::string> res;
    for (auto& d : _parsed) {
        res.push_back(d->name);
    }

    return res;
}

bool BinderParser::isVariableParsed(const std::string& variable) const
{
    assert(!variable.empty());
    return _parsed_hash.find(sl::Utils::toLower(variable.at(0) == ':' ? variable : ":" + variable)) == _parsed_hash.end();
}

void BinderParser::parse()
{
    bool commentFound = false; // найден комментарий
    bool commentLine = false; // комментарий в режиме строки (символы --)

    bool stringFound = false; // найдено начало строки sql (символ ')
    bool varFound = false; // найдено начало переменной
    int firstVarPos = -1;

    for (int i = 0; i < _sql_template.length(); i++) {
        char c = _sql_template.at(i);

        if (commentFound) {
            //состояние поиска окончания комментария
            if (c == '\n' && commentLine) {
                commentFound = false;
                commentLine = false;
            }
            if (c == '*' && (i == _sql_template.length() - 1 || _sql_template.at(i + 1) == '/')) {
                if (i != _sql_template.length() - 1)
                    i++; // переходим на символ вперед, чтобы пропустить '/'
                commentFound = false;
                commentLine = false;
            }

            continue;
        }

        if (stringFound) {
            // В состоянии поиска закрытия строки
            if (c == '\'') {
                // Найдена потенциальная закрывающая ковычка
                if (i < _sql_template.length() - 1 && _sql_template.at(i + 1) == '\'') {
                    // Это двойная одиночная ковычка - пропускаем и переходим на один символ вперед
                    i++;
                    continue;
                }
                stringFound = false;
            }
            continue;
        }

        if (c == '\'') {
            // Найдена открывающая ковычка
            stringFound = true;
        }

        if (varFound) {
            // В режиме поиска конца переменной
            bool isAlnum = sl::Utils::isAlnum(c);
            if (stringFound || (i == _sql_template.length() - 1 || !isAlnum)) {
                // В конце строки или найден не алфавитно-цифровой символ
                auto d = std::make_shared<Data>();
                d->name = sl::Utils::toLower(_sql_template.substr(firstVarPos, i - firstVarPos));

                // Завершающий символ переменной в конце строки
                if (i == _sql_template.length() - 1 && isAlnum && !stringFound)
                    d->name += std::tolower(c);

                d->position = firstVarPos;
                _parsed.push_back(d);
                _parsed_hash[sl::Utils::toLower(d->name)] = d;

                varFound = false;
                assert(sl::Utils::trim_copy(d->name) != ":");
            }
            continue;
        }

        if (!stringFound && c == ':' && i != _sql_template.length() - 1 && _sql_template.at(i + 1) == ':') {
            // найдено ::
            i++;
            continue;
        }

        if (!stringFound && c == ':') {
            // найдено начало переменной
            varFound = true;
            firstVarPos = i;
            continue;
        }

        if (c == '/' && i != _sql_template.length() - 1 && _sql_template.at(i + 1) == '*') {
            // Начало многострочного комментария
            commentFound = true;
            commentLine = false;
            i++;
            continue;
        }

        if (c == '-' && i != _sql_template.length() - 1 && _sql_template.at(i + 1) == '-') {
            // Начало однострочного комментария
            commentFound = true;
            commentLine = true;
            i++;
            continue;
        }
    }
}

Binder::Binder()
{
}

Binder::Binder(const std::string& sql_template)
    : _sql_template(sl::Utils::trim_copy(sql_template))
{
}

Binder::~Binder()
{
    clear();
}

bool Binder::isBinded(const std::string& variable) const
{
    assert(isPrepared());

    std::string var = sl::Utils::trim_copy(variable);
    assert(!var.empty());

    if (var.at(0) != ':')
        var = ":" + var;
    return _values.find(var) != _values.end();
}

bool Binder::isSqlCalculated() const
{
    return _data != nullptr;
}

bool Binder::isPrepared() const
{
    return !_sql_template.empty();
}

void Binder::prepare(const std::string& sql_template)
{
    clear();
    _sql_template = sl::Utils::trim_copy(sql_template);
    assert(!_sql_template.empty());
}

void Binder::bind(const std::string& variable, bool value)
{
    bind(variable, value ? "TRUE" : "FALSE", false, false);
}

void Binder::bind(const std::string& variable, int16_t value)
{
    bind(variable, std::to_string(value), false, false);
}

void Binder::bind(const std::string& variable, uint16_t value)
{
    bind(variable, std::to_string(value), false, false);
}

void Binder::bind(const std::string& variable, int64_t value)
{
    bind(variable, std::to_string(value), false, false);
}

void Binder::bind(const std::string& variable, uint64_t value)
{
    bind(variable, std::to_string(value), false, false);
}

void Binder::bind(const std::string& variable, double value)
{
    bind(variable, std::to_string(value), false, false);
}

void Binder::bind(const std::string& variable, float value)
{
    bind(variable, std::to_string(value), false, false);
}

void Binder::bind(const std::string& variable, const std::string& value, bool trim_string, bool format_string)
{
    assert(isPrepared());

    std::string var = sl::Utils::toLower(variable);
    assert(!var.empty());
    assert(!_is_calculated);

    if (_values.find(var) != _values.end())
        return;

    if (var.at(0) != ':')
        var = ":" + var;

    std::string val = value;
    if (trim_string)
        sl::Utils::trim(val);

    if (format_string) {
        if (val.empty())
            val = "NULL";
        else
            val = "E'" + val + "'";
    }

    _values[var] = val;
}

void Binder::bind(const std::string& variable, const char* value, bool trim_string, bool format_string)
{
    bind(variable, std::string(value), trim_string, format_string);
}

void Binder::unbind(const std::string& variable)
{
    assert(isBinded(variable));

    auto var = sl::Utils::trim_copy(variable);
    assert(!var.empty());

    if (var.at(0) != ':')
        var = ":" + var;

    _values.erase(var);
}

std::string Binder::sql(bool halt_if_not_used) const
{
    assert(isPrepared());
    if (!_data)
        _data = std::make_unique<BinderParser>(_sql_template);

    if (!_is_calculated)
        _sql = data()->calculate(_values, halt_if_not_used);

    _is_calculated = true;

    return _sql;
}

void Binder::clearBinds()
{
    _values.clear();
    _is_calculated = false;
}

std::vector<std::string> Binder::parsedVariables() const
{
    if (!isPrepared())
        return {};

    return data()->parsedVariables();
}

bool Binder::isVariableParsed(const std::string& variable) const
{
    assert(isPrepared());
    return data()->isVariableParsed(variable);
}

std::string Binder::bindOne(const std::string& sql_template, const std::string& variable, bool value)
{
    return bindOne(sql_template, variable, value ? "TRUE" : "FALSE", false, false);
}

std::string Binder::bindOne(const std::string& sql_template, const std::string& variable, int16_t value)
{
    return bindOne(sql_template, variable, std::to_string(value), false, false);
}

std::string Binder::bindOne(const std::string& sql_template, const std::string& variable, uint16_t value)
{
    return bindOne(sql_template, variable, std::to_string(value), false, false);
}

std::string Binder::bindOne(const std::string& sql_template, const std::string& variable, int64_t value)
{
    return bindOne(sql_template, variable, std::to_string(value), false, false);
}

std::string Binder::bindOne(const std::string& sql_template, const std::string& variable, uint64_t value)
{
    return bindOne(sql_template, variable, std::to_string(value), false, false);
}

std::string Binder::bindOne(const std::string& sql_template, const std::string& variable, double value)
{
    return bindOne(sql_template, variable, std::to_string(value), false, false);
}

std::string Binder::bindOne(const std::string& sql_template, const std::string& variable, float value)
{
    return bindOne(sql_template, variable, std::to_string(value), false, false);
}

std::string Binder::bindOne(const std::string& sql_template, const std::string& variable, const std::string& value, bool trim_string,
                            bool format_string)
{
    Binder binder(sql_template);
    binder.bind(variable, value, trim_string, format_string);
    return binder.sql();
}

std::string Binder::bindOne(const std::string& sql_template, const std::string& variable, const char* value, bool trim_string,
                            bool format_string)
{
    return bindOne(sql_template, variable, std::string(value), trim_string);
}

void Binder::clear()
{
    _data.reset();
    _sql_template.clear();
    _values.clear();
    _sql.clear();
    _is_calculated = false;
}

BinderParser* Binder::data() const
{
    if (!_data)
        _data = std::make_unique<BinderParser>(_sql_template);
    return _data.get();
}

} // namespace sql
