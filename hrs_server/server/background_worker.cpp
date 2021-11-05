#include "background_worker.h"
#include <utils/sl_utils.h>
#include <assert.h>

namespace hrs
{
BackgroundWorker::BackgroundWorker()
{
}

BackgroundWorker::~BackgroundWorker()
{
    stop();
}

void BackgroundWorker::start()
{
    assert(_thread == nullptr);
    // запускаем поток обработки
    _thread = new std::thread([&]() { process(); });
}

void BackgroundWorker::stop()
{
    if (_thread == nullptr)
        return;

    // запрашиваем остановку
    stopRequest();
    sl::Utils::coutPrint("background worker stopping...");
    // ждем завершения
    _thread->join();
    sl::Utils::coutPrint("background worker stopped");
    delete _thread;
    _thread = nullptr;
}

void BackgroundWorker::process()
{
    while (!isStopRequested()) {
        // пока ничего не делает
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

} // namespace hrs
