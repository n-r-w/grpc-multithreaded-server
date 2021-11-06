#include "srv_test.h"
#include "../hrs_factory.h"
#include <utils/sl_utils.h>
#include <api/src/hrs_data_converter.h>

namespace hrs
{
bool TestRequestProcessor1::handleRequest()
{
    // запрос
    sl::Utils::coutPrint("TestRequestProcessor1: " + request()->request1_info());

    double xxx = 0;
    size_t counter = 99999999;
    for (size_t i = 0; i < counter; i++) {
        xxx += counter / (i + 1);
    }

    reply()->set_reply1_info("наш ответ 1: " + std::to_string(xxx));

    return true;
}
} // namespace hrs
