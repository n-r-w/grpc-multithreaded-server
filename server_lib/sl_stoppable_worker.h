#pragma once

#include <thread>
#include <mutex>
#include <future>

namespace sl
{
// Базовый класс для корректного завершения потока
class StoppableWorker
{
public:
    StoppableWorker();

    // Был ли запрос на остановку потока
    bool isStopRequested() const;
    // Запросить остановку потока
    void stopRequest();

private:
    std::promise<void> _exit_signal;
    std::future<void> _exit_trigger;
};
} // namespace hrs
