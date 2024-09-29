set(HAKO_CMAKE_VERSION 3.5.1)
cmake_minimum_required(VERSION ${HAKO_CMAKE_VERSION})

project(hakoniwa-core
    VERSION 1.0.0
    DESCRIPTION "hakoniwa-core-client project"
    LANGUAGES C CXX
)

enable_testing()

set(HAKO_REPO_SOURCE_DIR  "${PROJECT_SOURCE_DIR}/../")
set(HAKO_CORE_SOURCE_DIR "${HAKO_REPO_SOURCE_DIR}/core")
set(HAKO_CMD_SOURCE_DIR "${HAKO_REPO_SOURCE_DIR}/src/cmd")
set(HAKO_LIB_SOURCE_DIR "${HAKO_REPO_SOURCE_DIR}/src/lib")
set(HAKO_PROXY_SOURCE_DIR "${HAKO_REPO_SOURCE_DIR}/src/proxy")
set(HAKO_INC_DIR "${HAKO_REPO_SOURCE_DIR}/src/include")
set(HAKOC_SOURCE_DIR "${HAKO_REPO_SOURCE_DIR}/src/hakoc/src")
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
    include(../default-cmake-options.cmake)
endif()

if(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
    # macOS (Darwin) の場合
    if(DEFINED ENV{DYLD_LIBRARY_PATH})
        # DYLD_LIBRARY_PATHを分割
        string(REPLACE ":" ";" LIBRARY_PATH_LIST $ENV{DYLD_LIBRARY_PATH})
        foreach(LIB_PATH IN LISTS LIBRARY_PATH_LIST)
            link_directories(${LIB_PATH})
            message(STATUS "Linking to: ${LIB_PATH} on macOS")
        endforeach()
    else()
        message(STATUS "DYLD_LIBRARY_PATH is not defined on macOS")
    endif()
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    # Linuxの場合
    if(DEFINED ENV{LD_LIBRARY_PATH})
        # LD_LIBRARY_PATHを分割
        string(REPLACE ":" ";" LIBRARY_PATH_LIST $ENV{LD_LIBRARY_PATH})
        foreach(LIB_PATH IN LISTS LIBRARY_PATH_LIST)
            link_directories(${LIB_PATH})
            message(STATUS "Linking to: ${LIB_PATH} on Linux")
        endforeach()
    else()
        message(STATUS "LD_LIBRARY_PATH is not defined on Linux")
    endif()
else()
    message(STATUS "Unsupported OS: ${CMAKE_SYSTEM_NAME}")
endif()


message(STATUS "BUILD_TYPE" ${BUILD_TYPE})
message(STATUS "GCOV" ${GCOV})

add_subdirectory(examples)



