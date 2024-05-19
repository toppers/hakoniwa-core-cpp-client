cmake_minimum_required(VERSION 3.10)

# Set the project name and the version of C++ to use.
project(hako_asset_test_project VERSION 1.0 LANGUAGES CXX)

# Set the C++ standard.
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(HOME_DIR ${TEST_DIR}/../..)

# Download and build Google Test.
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.10.0
)
FetchContent_MakeAvailable(googletest)

include_directories(
  ${HOME_DIR}/src/include
  ${HOME_DIR}/core/src/include
  ${HOME_DIR}/core/third-party/json/include
)

# Specify the source code for the test and the test code.
set(SOURCE_FILES
  ${HOME_DIR}/src/assets/src/hako_asset.cpp
  ${HOME_DIR}/src/assets/src/hako_asset_impl.cpp
  ./test.cpp
)

link_directories(/usr/local/lib/hakoniwa)

# Generate an executable file.
add_executable(test ${SOURCE_FILES})

# Link with the Google Test and hakoniwa core library.
target_link_libraries( test
  gtest_main
  assets
  conductor
)