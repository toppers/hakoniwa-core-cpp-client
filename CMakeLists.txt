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
add_subdirectory(src/proxy)
add_subdirectory(src/hakoc)

#if (test)
#    add_subdirectory(test)
#endif()


