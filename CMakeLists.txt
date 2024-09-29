set(HAKO_CMAKE_VERSION 3.5.1)
cmake_minimum_required(VERSION ${HAKO_CMAKE_VERSION})

project(hakoniwa-core
    VERSION 1.0.0
    DESCRIPTION "hakoniwa-core-client project"
    LANGUAGES C CXX
)

enable_testing()

set(HAKO_CORE_SOURCE_DIR "${PROJECT_SOURCE_DIR}/core")
set(HAKO_CMD_SOURCE_DIR "${PROJECT_SOURCE_DIR}/src/cmd")
set(HAKO_LIB_SOURCE_DIR "${PROJECT_SOURCE_DIR}/src/lib")
set(HAKO_PROXY_SOURCE_DIR "${PROJECT_SOURCE_DIR}/src/proxy")
set(HAKO_INC_DIR "${PROJECT_SOURCE_DIR}/src/include")
set(HAKOC_SOURCE_DIR "${PROJECT_SOURCE_DIR}/src/hakoc/src")
# ENABLE_HAKO_TIME_MEASURE オプションを追加 (デフォルトはOFF)
option(ENABLE_HAKO_TIME_MEASURE "Enable time measurement feature" OFF)
if(ENABLE_HAKO_TIME_MEASURE)
message(STATUS "Building with ENABLE_HAKO_TIME_MEASURE")
add_definitions(-DENABLE_HAKO_TIME_MEASURE)
set(HAKO_MEASURE_SRC_DIR "${PROJECT_SOURCE_DIR}/src/assets/src")
endif()

if (DEFINED HAKO_CLIENT_OPTION_FILEPATH)
    include(${HAKO_CLIENT_OPTION_FILEPATH})
else()
    include(default-cmake-options.cmake)
endif()

message(STATUS "BUILD_TYPE" ${BUILD_TYPE})
message(STATUS "GCOV" ${GCOV})

#add_subdirectory(src/cmd)
#add_subdirectory(src/lib)
add_subdirectory(core)
if(WIN32)
else(WIN32)
add_subdirectory(src/proxy)
endif(WIN32)
add_subdirectory(src/hakoc)
add_subdirectory(src/assets)
add_subdirectory(src/conductor)
add_subdirectory(bindings)
add_subdirectory(examples)

#if (test)
#    add_subdirectory(test)
#endif()


