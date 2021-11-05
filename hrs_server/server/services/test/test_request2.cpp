#include "srv_test.h"
#include "../hrs_factory.h"
#include <utils/sl_utils.h>
#include <api/src/data_converter.h>

namespace hrs
{
void TestRequestProcessor2::handleRequest()
{
    // запрос
    sl::Utils::coutPrint("TestRequestProcessor2: " + request()->request2_info());

    double xxx = 0;
    size_t counter = 99999999;
    for (size_t i = 0; i < counter; i++) {
        xxx += counter / (i + 1);
    }
    reply()->set_reply2_info("наш ответ 2: " + std::to_string(xxx));
}

} // namespace hrs
