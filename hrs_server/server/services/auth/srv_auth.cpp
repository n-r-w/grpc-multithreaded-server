#include "srv_auth.h"

#include "../hrs_factory.h"
#include <utils/sl_utils.h>
#include "../../hrs_auth.h"
#include "../src/hrs_error_codes.h"

#include <atomic>

namespace hrs
{
bool LoginRequestProcessor::handleRequest()
{
    std::string session_id;
    if (HrsServiceFactory::instance()->userValidator()->login(request()->login(), request()->password(), session_id)) {
        reply()->set_error_code(ErrorCodes::NoError);
        reply()->set_session_id(session_id);
        reply()->set_keep_alive_sec(HrsServiceFactory::instance()->userValidator()->expire().count());

    } else {
        reply()->set_error_code(ErrorCodes::AuthFail);
    }
    return true;
}

bool KeepAliveRequestProcessor::handleRequest()
{
    if (!HrsServiceFactory::instance()->userValidator()->checkSession(request()->session_id()))
        reply()->set_error_code(ErrorCodes::AuthFail);
    else
        reply()->set_error_code(ErrorCodes::NoError);
    return true;
}

} // namespace hrs
