cmake_minimum_required(VERSION 3.12)
project(SimpleBackendServiceCpp)

# Packages
find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)

# Proto files
set(PROTO_FILES
    proto/jobs.proto
)

# Executables
add_executable(SimpleBackendServiceCpp
  "src/server/main.cpp"
  "src/core/job.cpp" "src/core/job_queue.cpp" "src/core/executor.cpp" 
  "src/intermediate/job_repository.cpp" "src/intermediate/job_submission_service.cpp")

# Include generated protobuff files
target_include_directories(SimpleBackendServiceCpp PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

# Linked libraries
target_link_libraries(SimpleBackendServiceCpp PRIVATE
  protobuf::libprotoc
  protobuf::libprotobuf
  gRPC::grpc
  gRPC::grpc++
)

set_property(TARGET SimpleBackendServiceCpp PROPERTY CXX_STANDARD 20)

get_target_property(grpc_cpp_plugin_location gRPC::grpc_cpp_plugin LOCATION)
protobuf_generate(TARGET SimpleBackendServiceCpp LANGUAGE cpp PROTOS ${PROTO_FILES})
protobuf_generate(TARGET SimpleBackendServiceCpp LANGUAGE grpc GENERATE_EXTENSIONS .grpc.pb.h .grpc.pb.cc PLUGIN "protoc-gen-grpc=${grpc_cpp_plugin_location}" PROTOS ${PROTO_FILES})
