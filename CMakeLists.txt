cmake_minimum_required(VERSION 3.5.1)

set(ID test_srv_project)
project(${ID} C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(TOP_SRS_DIR ${PROJECT_SOURCE_DIR})
set(BIN_DIR ${PROJECT_SOURCE_DIR}/bin)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${BIN_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${BIN_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${BIN_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${BIN_DIR})
link_directories(${BIN_DIR})

#add_subdirectory(sql_lib)
add_subdirectory(server_lib)
add_subdirectory(hrs_server)

