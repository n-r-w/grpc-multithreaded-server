set(ID hrs_api)
project(${ID} C CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# файлы с описанием API (расширение proto) в каталоге api/protos
set(PROTO_FILES
    shared
    srv
    sql
    test
)

file(GLOB SOURCES
    "src/*.h"
    "src/*.cpp"
)

# макро для генерации кода на основе прото файлов
macro(generate_proto_api proto_file)
    execute_process(COMMAND mkdir -p ${API_DIR}/${proto_file})
    add_custom_command(
          OUTPUT "${API_DIR}/${proto_file}/${proto_file}.pb.cc" "${API_DIR}/${proto_file}/${proto_file}.pb.h" "${API_DIR}/${proto_file}/${proto_file}.grpc.pb.cc" "${API_DIR}/${proto_file}/${proto_file}.grpc.pb.h"
          COMMAND ${_PROTOBUF_PROTOC}
          ARGS --grpc_out "${API_DIR}/${proto_file}"
            --cpp_out "${API_DIR}/${proto_file}"
            -I "${PROTOS_DIR}"
            --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
            "${PROTOS_DIR}/${proto_file}.proto"
          DEPENDS "${PROTOS_DIR}/${proto_file}.proto")
endmacro()

# генерим api и добавляем созданные файлы в проект
foreach(f_name ${PROTO_FILES})
    list(APPEND PROTOS_FILES ${PROTOS_DIR}/${f_name}.proto)

    generate_proto_api(${f_name})

    list(APPEND API_FILES_GENERATED_FILES ${API_DIR}/${f_name}/${f_name}.pb.cc)
    list(APPEND API_FILES_GENERATED_FILES ${API_DIR}/${f_name}/${f_name}.pb.h)
    list(APPEND API_FILES_GENERATED_FILES ${API_DIR}/${f_name}/${f_name}.grpc.pb.cc)
    list(APPEND API_FILES_GENERATED_FILES ${API_DIR}/${f_name}/${f_name}.grpc.pb.h)
endforeach()

add_library(${ID} SHARED
    ${API_FILES_GENERATED_FILES}
    ${SOURCES}
    ${PROTOS_FILES}
)

source_group("src" FILES ${SOURCES})
source_group("generated" FILES ${API_FILES_GENERATED_FILES})
source_group("protos" FILES ${PROTOS_FILES})

target_include_directories(${ID} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${ID}
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF})
