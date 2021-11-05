#include "srv_sql.h"
#include "../hrs_factory.h"
#include <utils/sl_utils.h>
#include <api/src/data_converter.h>

namespace hrs
{
void SqlRequestProcessor::handleRequest()
{
    sl::Utils::coutPrint("SqlRequestProcessor: " + request()->sql());

    double xxx = 0;
    size_t counter = 99999999;
    for (size_t i = 0; i < counter; i++) {
        xxx += counter / (i + 1);
    }

    reply()->set_error_text(std::to_string(xxx));
}
} // namespace hrs
