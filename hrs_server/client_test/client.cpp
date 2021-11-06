#include <iostream>
#include <memory>
#include <string>
#include <thread>

#include <utils/sl_utils.h>
#include <api/src/hrs_data_converter.h>
#include <client_lib/hrs_channel.h>
#include <client_lib/hrs_sql_request.h>

#include <grpcpp/grpcpp.h>
#include <api/generated/srv/srv.grpc.pb.h>

void execSqlClient()
{
    size_t counter = 0;
    while (true) {
        sl::Error error;

        auto result = hrs::SqlRequest::exec("SELECT * FROM r0031", error);
        if (error.isError()) {
            sl::Utils::coutPrint(std::to_string(++counter) + ", " + error.text());

        } else {
            assert(result->type() == sql::Query::ResultType::Select);
            sl::Utils::coutPrint(std::to_string(++counter) + ", SQL data received. Row count: " + std::to_string(result->data()->rowCount())
                                 + ", column count: " + std::to_string(result->data()->columnCount()));
        }

        result = hrs::SqlRequest::exec("UPDATE c_types SET sp=1 WHERE p00='1'", error);
        if (error.isError()) {
            sl::Utils::coutPrint(std::to_string(++counter) + ", " + error.text());

        } else {
            assert(result->type() == sql::Query::ResultType::Command);
            sl::Utils::coutPrint(std::to_string(++counter) + ", SQL data received. Command OK");
        }
    }
}

int main(int argc, char** argv)
{
    auto error = hrs::ClientChannel::connect("localhost:50051", grpc::InsecureChannelCredentials(), "petrov", "qq1234");
    if (error.isError()) {
        sl::Utils::coutPrint(std::to_string(error.code()));
        return 0;
    }

    size_t worker_sql_count = 50;
    std::vector<std::thread*> worker_threads;

    for (size_t i = 0; i < worker_sql_count; i++) {
        std::thread* worker_thread = new std::thread([]() { execSqlClient(); });
        worker_threads.push_back(worker_thread);
    }

    for (size_t i = 0; i < worker_threads.size(); ++i) {
        worker_threads.at(i)->join();
    }

    return 0;
}
