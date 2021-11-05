#pragma once

namespace hrs
{
//! Коды ошибок
struct ErrorCodes
{
    enum _ErrorCodes
    {
        NoError = 0,
        //! Соединение с сервером не удалось или потеряно
        ConnectionFail,
        //! Неверный логин или пароль
        AuthFail,
        //! Запрос к серверу не выполнен
        RpcFail,
    };
};

} // namespace hrs
