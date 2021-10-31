#include "srv_test.h"
#include "../hrs_factory.h"
#include <server_lib/sl_utils.h>
#include <api/src/data_converter.h>

namespace hrs
{
void TestRequestProcessor2::handleRequest()
{ // _request.sql() ...

    sl::Utils::coutPrint("TestRequestProcessor2::process");

    double xxx = 0;
    size_t counter = 999999999;
    for (size_t i = 0; i < counter; i++) {
        xxx += counter / (i + 1);
    }
    sl::Utils::coutPrint("TestRequestProcessor2::finish" + std::to_string(xxx));

    //    _reply.set_allocated_data(.....);
}

} // namespace hrs