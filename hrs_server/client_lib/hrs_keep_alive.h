#pragma once

#include <utils/sl_stoppable_worker.h>

namespace hrs
{
class KeepAliveWorker : public sl::StoppableWorker
{
public:
    KeepAliveWorker(const std::chrono::seconds& interval);
    ~KeepAliveWorker();

    void start();
    void stop();

private:
    //!  Обработчик
    void process();

    // Блокировка
    mutable std::mutex _locker;
    // Поток, в котором запускается
    std::thread* _thread = nullptr;
    std::chrono::seconds _interval;
};
} // namespace hrs
