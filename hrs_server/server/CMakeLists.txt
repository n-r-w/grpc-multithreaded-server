set(ID hrs_server)
project(${ID} C CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SOURCES
    "*.h"
    "*.cpp"
)

file(GLOB_RECURSE SERVICES
    "services/*.h"
    "services/*.cpp"
)

include_directories(
    ${PROJECT_SOURCE_DIR}
)

add_executable(${ID}
    ${SOURCES}
    ${SERVICES}
)

target_link_libraries(${ID}
    server_lib
    hrs_api
    ${_REFLECTION}
    ${_GRPC_GRPCPP}
    ${_PROTOBUF_LIBPROTOBUF})
