#include "sl_stoppable_worker.h"

namespace sl
{
StoppableWorker::StoppableWorker()
    : _exit_trigger(_exit_signal.get_future())
{
}

bool StoppableWorker::isStopRequested() const
{
    return _exit_trigger.wait_for(std::chrono::milliseconds(0)) != std::future_status::timeout;
}

void StoppableWorker::stopRequest()
{
    if (isStopRequested())
        return;

    _exit_signal.set_value();
}

} // namespace hrs
