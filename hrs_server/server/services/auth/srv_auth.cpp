#include "srv_auth.h"

#include "../hrs_factory.h"
#include <utils/sl_utils.h>
#include "../../hrs_auth.h"
#include "../src/hrs_error_codes.h"

namespace hrs
{
void AuthRequestProcessor::handleRequest()
{
    if (HrsServiceFactory::instance()->userValidator()->checkUser(request()->login(), request()->password())) {
        reply()->set_error_code(ErrorCodes::NoError);

    } else {
        reply()->set_error_code(ErrorCodes::AuthFail);
    }
}
} // namespace hrs
