# Copyright 2018 gRPC authors.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# cmake build file for C++ sample example.
# Assumes protobuf and gRPC have been installed using cmake.
# See cmake_externalproject/CMakeLists.txt for all-in-one cmake build
# that automatically builds all the dependencies before building sample.

cmake_minimum_required(VERSION 3.5.1)

project(HakoClient C CXX)

if(NOT MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
else()
  add_definitions(-D_WIN32_WINNT=0x600)
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
find_package(Threads REQUIRED)

# This branch assumes that gRPC and all its dependencies are already installed
# on this system, so they can be located by find_package().

# Find Protobuf installation
# Looks for protobuf-config.cmake file installed by Protobuf's cmake installation.
set(protobuf_MODULE_COMPATIBLE TRUE)
find_package(Protobuf CONFIG REQUIRED)
message(STATUS "Using protobuf ${Protobuf_VERSION}")

set(_PROTOBUF_LIBPROTOBUF protobuf::libprotobuf)
set(_REFLECTION gRPC::grpc++_reflection)
if(CMAKE_CROSSCOMPILING)
  find_program(_PROTOBUF_PROTOC protoc)
else()
  set(_PROTOBUF_PROTOC $<TARGET_FILE:protobuf::protoc>)
endif()

# Find gRPC installation
# Looks for gRPCConfig.cmake file installed by gRPC's cmake installation.
find_package(gRPC CONFIG REQUIRED)
message(STATUS "Using gRPC ${gRPC_VERSION}")

set(_GRPC_GRPCPP gRPC::grpc++)
if(CMAKE_CROSSCOMPILING)
  find_program(_GRPC_CPP_PLUGIN_EXECUTABLE grpc_cpp_plugin)
else()
  set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:gRPC::grpc_cpp_plugin>)
endif()

# Proto file
get_filename_component(hakoniwa_proto "${PROJECT_SOURCE_DIR}/../../hakoniwa-core-spec/grpc/hakoniwa_core.proto" ABSOLUTE)
get_filename_component(hakoniwa_proto_path "${hakoniwa_proto}" PATH)

# Generated sources
set(hakoniwa_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/hakoniwa_core.pb.cc")
set(hakoniwa_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/hakoniwa_core.pb.h")
set(hakoniwa_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/hakoniwa_core.grpc.pb.cc")
set(hakoniwa_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/hakoniwa_core.grpc.pb.h")
add_custom_command(
      OUTPUT "${hakoniwa_proto_srcs}" "${hakoniwa_proto_hdrs}" "${hakoniwa_grpc_srcs}" "${hakoniwa_grpc_hdrs}"
      COMMAND ${_PROTOBUF_PROTOC}
      ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I "${hakoniwa_proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${hakoniwa_proto}"
      DEPENDS "${hakoniwa_proto}")

# Include generated *.pb.h files
include_directories(
  "${CMAKE_CURRENT_BINARY_DIR}"
  "${PROJECT_SOURCE_DIR}/.."
  "${PROJECT_SOURCE_DIR}/../../hakoniwa-core-cpp/third-party/json/single_include"
)

foreach(_target hako_rpc_client)
  add_library(${_target} STATIC "${PROJECT_SOURCE_DIR}/../lib/rpc/${_target}.cpp"
    ${hakoniwa_proto_srcs}
    ${hakoniwa_grpc_srcs})
  target_link_libraries(${_target}
    ${_REFLECTION}
    ${_GRPC_GRPCPP}
    ${_PROTOBUF_LIBPROTOBUF})
endforeach()

add_executable(hakoniwa_proxy ${PROJECT_SOURCE_DIR}/rpc/hako_proxy.cpp)
target_link_libraries(hakoniwa_proxy hako_rpc_client
    ${_REFLECTION}
    ${_GRPC_GRPCPP}
    ${_PROTOBUF_LIBPROTOBUF})
