cmake_minimum_required(VERSION 3.5.1)

set(ID hrs_server_project)
project(${ID} C CXX)

list(APPEND CMAKE_PREFIX_PATH ${TOP_SRS_DIR}/prebuilds/grpc/bin/)
list(APPEND CMAKE_PREFIX_PATH ${TOP_SRS_DIR}/prebuilds/grpc/lib/cmake/protobuf/)
include(${TOP_SRS_DIR}/cmake_tools/grpc.cmake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROTOS_DIR ${PROJECT_SOURCE_DIR}/api/protos)
set(API_DIR ${PROJECT_SOURCE_DIR}/api/generated)

include_directories(
    ${TOP_SRS_DIR}
    ${API_DIR}
    ${API_DIR}/shared
    ${PROJECT_SOURCE_DIR}
)

add_subdirectory(api)
add_subdirectory(server)
add_subdirectory(client_test)

