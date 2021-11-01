#include <iostream>
#include <memory>
#include <string>
#include <thread>

#include <grpcpp/grpcpp.h>

#include <api/generated/shared/shared.grpc.pb.h>
#include <api/generated/sql/sql.grpc.pb.h>
#include <api/generated/test/test.grpc.pb.h>

#include <grpcpp/security/auth_context.h>

#include <server_lib/sl_utils.h>
#include <server_lib/sl_auth.h>

#include <hrs_server/server/hrs_auth.h>

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

class SqlServiceClient
{
public:
    SqlServiceClient(std::shared_ptr<Channel> channel, uint id)
        : _stub(SqlApi::Sql::NewStub(channel))
    {
    }

    std::string executeSQL(const std::string& sql)
    {
        SqlApi::SqlRequest request;
        request.set_sql(sql);

        SqlApi::SqlReply reply;

        ClientContext context;

        context.AddMetadata(hrs::UserValidator::LOGIN_METADATA, "petrov");
        context.AddMetadata(hrs::UserValidator::PASSWORD_METADATA, "qq1234");

        Status status = _stub->ExecuteSQL(&context, request, &reply);

        if (status.ok()) {
            // ответ в reply.data()
            return "SQL data received";

        } else {
            return std::to_string(status.error_code()) + ": " + status.error_message();
        }
    }

private:
    std::unique_ptr<SqlApi::Sql::Stub> _stub;
};

class TestServiceClient
{
public:
    TestServiceClient(std::shared_ptr<Channel> channel, uint id)
        : _stub(TestApi::TestService::NewStub(channel))
    {
    }

    std::string executeTestRequest(const std::string& info, int test_num)
    {
        TestApi::TestRequest request;
        request.set_info(info);

        TestApi::TestReply reply;

        ClientContext context;

        context.AddMetadata(hrs::UserValidator::LOGIN_METADATA, "petrov");
        context.AddMetadata(hrs::UserValidator::PASSWORD_METADATA, "qq1234");

        Status status;
        if (test_num == 1)
            status = _stub->ExecuteTestRequest1(&context, request, &reply);
        else
            status = _stub->ExecuteTestRequest2(&context, request, &reply);

        if (status.ok()) {
            // ответ в reply.data()
            return "Test data received";

        } else {
            return "error: " + std::to_string(status.error_code()) + ": " + status.error_message();
        }
    }

private:
    std::unique_ptr<TestApi::TestService::Stub> _stub;
};

void execSqlClient(const std::string& info, uint id)
{
    size_t counter = 0;
    while (true) {
        sl::Utils::coutPrint("SQL request start: " + std::to_string(id) + ", count: " + std::to_string(++counter));

        SqlServiceClient service_client(grpc::CreateChannel(info, grpc::InsecureChannelCredentials()), id);

        std::string status = service_client.executeSQL("SELECT * FROM XXXX");

        sl::Utils::coutPrint(status + ": " + std::to_string(id) + ", count: " + std::to_string(++counter));
    }
}

void execTestClient(const std::string& info, uint id, int test_num)
{
    size_t counter = 0;
    while (true) {
        sl::Utils::coutPrint("Test request start: " + std::to_string(id) + "," + std::to_string(test_num)
                             + ", count: " + std::to_string(++counter));

        auto channel = grpc::CreateChannel(info, grpc::InsecureChannelCredentials());

        TestServiceClient test_client(channel, id);

        std::string status = test_client.executeTestRequest("hello", test_num);

        sl::Utils::coutPrint(status + ": " + std::to_string(id) + ", count: " + std::to_string(++counter));
    }
}

int main(int argc, char** argv)
{
    std::string target_str = "localhost:50051";

    size_t worker_sql_count = 0;
    size_t worker_test1_count = 100;
    size_t worker_test2_count = 0;

    std::vector<std::thread*> worker_threads;

    for (size_t i = 0; i < worker_test1_count; i++) {
        std::thread* worker_thread = new std::thread([target_str, i]() { execTestClient(target_str, i, 1); });
        worker_threads.push_back(worker_thread);
    }

    for (size_t i = 0; i < worker_test2_count; i++) {
        std::thread* worker_thread = new std::thread([target_str, i]() { execTestClient(target_str, i, 2); });
        worker_threads.push_back(worker_thread);
    }

    for (size_t i = 0; i < worker_sql_count; i++) {
        std::thread* worker_thread = new std::thread([target_str, i]() { execSqlClient(target_str, i); });
        worker_threads.push_back(worker_thread);
    }

    for (size_t i = 0; i < worker_threads.size(); ++i) {
        worker_threads.at(i)->join();
    }

    return 0;
}
