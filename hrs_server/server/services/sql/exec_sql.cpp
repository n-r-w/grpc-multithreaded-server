#include "srv_sql.h"
#include "../hrs_factory.h"
#include <server_lib/sl_utils.h>
#include <api/src/data_converter.h>

namespace hrs
{
void SqlRequestProcessor::handleRequest()
{
    // _request.sql() ...

    sl::Utils::coutPrint("SqlRequestProcessor::process");

    double xxx = 0;
    size_t counter = 999999999;
    for (size_t i = 0; i < counter; i++) {
        xxx += counter / (i + 1);
    }
    sl::Utils::coutPrint("SqlRequestProcessor::process" + std::to_string(xxx));

    //    _reply.set_allocated_data(.....);
}
} // namespace hrs
