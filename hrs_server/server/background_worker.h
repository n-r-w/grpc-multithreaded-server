#pragma once

#include <server_lib/sl_stoppable_worker.h>

namespace hrs
{
//! Выполняет некую фоновую обработку
//! Корректно завершает свой поток в деструкторе
class BackgroundWorker : protected sl::StoppableWorker
{
public:
    BackgroundWorker();
    ~BackgroundWorker();

    void start();
    void stop();

private:
    //!  Обработчик
    void process();

    // Блокировка
    mutable std::mutex _locker;
    // Поток, в котором запускается
    std::thread* _thread = nullptr;
};

} // namespace hrs
