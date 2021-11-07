#include <iostream>
#include <memory>
#include <string>
#include <thread>

#include <sl_utils.h>
#include <sl_binder.h>

#include <api/src/hrs_data_converter.h>
#include <hrs_channel.h>
#include <hrs_sql_request.h>

#include <grpcpp/grpcpp.h>
#include <api/generated/srv/srv.grpc.pb.h>
#include <atomic>

void execSqlClient()
{
    static std::atomic<uint64_t> last_duration(0);

    size_t counter = 0;
    while (true) {
        auto begin = std::chrono::steady_clock::now();

        sl::Error error;
        auto result = hrs::SqlRequest::exec("SELECT * FROM some_data", error);
        if (error.isOk())
            result = hrs::SqlRequest::exec("UPDATE some_data SET name='test name' WHERE id=1", error);

        auto end = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();

        uint64_t diff;
        if (last_duration == 0) {
            last_duration = duration;
            diff = 0;

        } else {
            diff = std::abs((int64_t)(last_duration - duration));
            last_duration = duration;
        }

        if (error.isError())
            sl::Utils::coutPrint("error: " + error.fullText() + ", duration: " + std::to_string(duration)
                                 + ", diff: " + std::to_string(diff));
        else
            sl::Utils::coutPrint("ok, duration: " + std::to_string(duration) + ", diff: " + std::to_string(diff));
    }
}

static void channelCollback(sl::Error error)
{
    sl::Utils::coutPrint("Callback error: " + error.fullText());
}

uint64_t addUser(const std::string& login, const std::string& password, const std::string& name, sl::Error& error)
{
    sql::Binder binder("INSERT INTO users(login, name, password_hash) values (:login, :name, :password_hash) RETURNING id_user");
    binder.bind(":login", login);
    binder.bind(":name", name);
    binder.bind(":password_hash", sl::Utils::sha256(password));

    auto result = hrs::SqlRequest::exec(binder.sql(), error);
    if (error.isError())
        return 0;

    auto id = result->data()->toString(0, 0);
    return std::stoul(*id);
}

int main(int argc, char** argv)
{
    auto error = hrs::ClientChannel::connect("localhost:50051", grpc::InsecureChannelCredentials(), "ivanov", "qq1234");
    if (error.isError()) {
        sl::Utils::coutPrint(std::to_string(error.code()));
        return 0;
    }

    //    auto user_id = addUser("petrov", "qq1234", "Petr Petrov", error);

    hrs::ClientChannel::setCallback(channelCollback);

    size_t worker_sql_count = 100;
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
