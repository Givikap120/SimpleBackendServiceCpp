cmake_minimum_required(VERSION 3.12)
project(SimpleBackendServiceCpp)

# Packages
find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(httplib CONFIG REQUIRED)

# Proto files
set(PROTO_FILES
    proto/jobs.proto
)

# Core lib
add_library(SimpleBackendServiceCppLib
    "src/core/job.cpp" "src/core/job_queue.cpp" "src/core/executor.cpp" 
    "src/intermediate/job_repository.cpp" "src/intermediate/job_submission_service.cpp" 
    "src/server/rest_server.cpp"
)

# Linked libraries
target_link_libraries(SimpleBackendServiceCppLib PUBLIC
  protobuf::libprotoc
  protobuf::libprotobuf
  gRPC::grpc
  gRPC::grpc++
  nlohmann_json::nlohmann_json
  httplib::httplib
)

target_include_directories(SimpleBackendServiceCppLib PUBLIC
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Executable
add_executable(SimpleBackendServiceCpp src/server/main.cpp)

# Linked libraries
target_link_libraries(SimpleBackendServiceCpp PRIVATE SimpleBackendServiceCppLib)

set_property(TARGET SimpleBackendServiceCppLib PROPERTY CXX_STANDARD 20)
set_property(TARGET SimpleBackendServiceCpp PROPERTY CXX_STANDARD 20)

get_target_property(grpc_cpp_plugin_location gRPC::grpc_cpp_plugin LOCATION)
protobuf_generate(TARGET SimpleBackendServiceCpp LANGUAGE cpp PROTOS ${PROTO_FILES})
protobuf_generate(TARGET SimpleBackendServiceCpp LANGUAGE grpc GENERATE_EXTENSIONS .grpc.pb.h .grpc.pb.cc PLUGIN "protoc-gen-grpc=${grpc_cpp_plugin_location}" PROTOS ${PROTO_FILES})

enable_testing()
add_subdirectory(tests)
