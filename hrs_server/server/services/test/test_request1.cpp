#include "srv_test.h"
#include "../hrs_factory.h"
#include <server_lib/sl_utils.h>
#include <api/src/data_converter.h>

namespace hrs
{
void TestRequestProcessor1::handleRequest()
{
    //    auto login = context()->auth_context()->FindPropertyValues("x-custom-auth-login");
    for (auto i = context()->auth_context()->begin(); i != context()->auth_context()->end(); ++i) {
        auto prop = *i;
        auto first = prop.first;
        auto second = prop.second;
        sl::Utils::coutPrint("auth_context!!!");
    }
    // запрос
    auto request_info = request()->info();

    sl::Utils::coutPrint("TestRequestProcessor1::process");

    double xxx = 0;
    size_t counter = 99999999;
    for (size_t i = 0; i < counter; i++) {
        xxx += counter / (i + 1);
    }
    sl::Utils::coutPrint("TestRequestProcessor1::finish" + std::to_string(xxx));

    reply()->set_info("наш ответ");
}
} // namespace hrs
