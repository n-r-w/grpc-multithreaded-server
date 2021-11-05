#include "sl_utils.h"

namespace sl
{
//! блокировка параллельного вывода в консоль
std::mutex Utils::_cout_locker;

void Utils::coutPrint(const std::string& message)
{
    std::lock_guard<std::mutex> lock(_cout_locker);
    std::cout << message << std::endl;
}

} // namespace hrs
