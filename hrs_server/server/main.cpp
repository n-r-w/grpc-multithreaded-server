#include "hrs_server.h"
#include "background_worker.h"
#include <server_lib/sl_utils.h>

int main(int argc, char** argv)
{
    hrs::BackgroundWorker background;
    hrs::HrsServer server;

    sl::Utils::coutPrint("enter 'q' for quit");

    background.start();
    server.start("0.0.0.0:50051", 10);

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
