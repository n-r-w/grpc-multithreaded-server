#include "hrs_server.h"
#include "background_worker.h"
#include <utils/sl_utils.h>
#include <sql_lib/plugins/psql/psql_impl.h>
#include "services/hrs_factory.h"

int main(int argc, char** argv)
{
    hrs::BackgroundWorker background;
    hrs::HrsServer server(60);
    sl::Error error = server.initDatabaseConnection("127.0.0.1", 5432, "HRS", "postgres", "1", "");
    if (error.isError()) {
        sl::Utils::coutPrint("Database connection error: " + error.fullText());
        return 0;
    }

    sl::Utils::coutPrint("enter 'q' for quit, 'c' for clear connection pool");

    background.start();
    server.start("0.0.0.0:50051", 10);

    char input;
    while (true) {
        std::cin.get(input);        
        if (input == 'q')
            break;
        else if (input == 'c') {
            size_t count = hrs::HrsServiceFactory::instance()->sqlConnectionPool()->clear();
            sl::Utils::coutPrint("cleared " + std::to_string(count) + " connections");
        }
    }

    background.stop();
    server.stop();

    return 0;
}
