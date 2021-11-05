#include "hrs_server.h"
#include "background_worker.h"
#include <utils/sl_utils.h>
#include <sql_lib/plugins/psql/psql_impl.h>

int main(int argc, char** argv)
{
    hrs::BackgroundWorker background;
    hrs::HrsServer server;

    sl::Utils::coutPrint("enter 'q' for quit");

    background.start();
    server.start("0.0.0.0:50051", 5);

    char input;
    while (true) {
        std::cin.get(input);        
        if (input == 'q')
            break;
    }

    background.stop();
    server.stop();

    return 0;
}
