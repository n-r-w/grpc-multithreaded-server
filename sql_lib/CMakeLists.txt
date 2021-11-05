set(ID sql_lib)
project(${ID} C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES
    "*.h"
    "*.cpp"
)

add_library(${ID} SHARED
    ${SOURCES}
)

include_directories(
    ${PROJECT_SOURCE_DIR}
)

target_include_directories(${ID} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${ID}
    sl_utils
    pq
)
