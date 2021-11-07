set(ID client_test)
project(${ID} C CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${ID} "client.cpp")

target_link_libraries(${ID}
    client_lib
    server_lib
    hrs_api
    sql_lib
    ${_REFLECTION}
    ${_GRPC_GRPCPP}
    ${_PROTOBUF_LIBPROTOBUF})
