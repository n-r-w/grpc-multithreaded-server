set(ID client_lib)
project(${ID} C CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SOURCES
    "*.h"
    "*.cpp"
)

add_library(${ID}
	${SOURCES}
)

target_link_libraries(${ID}    
    hrs_api
    sl_utils
    ${_REFLECTION}
    ${_GRPC_GRPCPP}
    ${_PROTOBUF_LIBPROTOBUF})
