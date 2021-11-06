set(ID server_lib)
project(${ID} C CXX)

list(APPEND CMAKE_PREFIX_PATH ${TOP_SRS_DIR}/prebuilds/grpc/bin/)
list(APPEND CMAKE_PREFIX_PATH ${TOP_SRS_DIR}/prebuilds/grpc/lib/cmake/protobuf/)
include(${TOP_SRS_DIR}/cmake_tools/grpc.cmake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SOURCES
    "*.h"
    "*.cpp"    
    "read.me"
)

add_library(${ID} SHARED
    ${SOURCES}
)

target_include_directories(${ID} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${ID}
    sl_utils
    ${_REFLECTION}
    ${_GRPC_GRPCPP}
    ${_PROTOBUF_LIBPROTOBUF})
