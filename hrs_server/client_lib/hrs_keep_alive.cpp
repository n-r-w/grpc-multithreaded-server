#include "hrs_keep_alive.h"
#include "hrs_channel.h"

namespace hrs
{
KeepAliveWorker::KeepAliveWorker(const std::chrono::seconds& interval)
    : _interval(interval)
{
}

KeepAliveWorker::~KeepAliveWorker()
{
    stop();
}

void KeepAliveWorker::start()
{
    assert(_thread == nullptr);
    // запускаем поток обработки
    _thread = new std::thread([&]() { process(); });
}

void KeepAliveWorker::stop()
{
    if (_thread == nullptr)
        return;

    // запрашиваем остановку
    stopRequest();
    // ждем завершения
    _thread->join();
    delete _thread;
    _thread = nullptr;
}

void KeepAliveWorker::process()
{
    auto timer = std::chrono::system_clock::now();
    while (!isStopRequested()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        if (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - timer) > _interval) {
            timer = std::chrono::system_clock::now();
            if (ClientChannel::keepAlive().isError())
                stopRequest();
        }
    }
}

} // namespace hrs
