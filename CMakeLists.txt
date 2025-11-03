set(HAKO_CMAKE_VERSION 3.20)
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

if(APPLE AND CMAKE_HOST_SYSTEM_PROCESSOR STREQUAL "arm64")
    set(CMAKE_OSX_ARCHITECTURES "arm64")
endif()
message(STATUS "CMAKE_SYSTEM_PROCESSOR: ${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS "CMAKE_OSX_ARCHITECTURES: ${CMAKE_OSX_ARCHITECTURES}")

message(STATUS "BUILD_TYPE" ${BUILD_TYPE})
message(STATUS "GCOV" ${GCOV})

message(STATUS "CMAKE_C_FLAGS" ${CMAKE_C_FLAGS})
message(STATUS "CMAKE_CXX_FLAGS" ${CMAKE_CXX_FLAGS})
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
if(NOT "${CMAKE_CXX_FLAGS}" MATCHES "-m32")
add_subdirectory(bindings)
add_subdirectory(examples)
endif()

#if (test)
#    add_subdirectory(test)
#endif()

# ---------------------------------------------
# Documentation (Doxygen)
# ---------------------------------------------
option(BUILD_DOCS "Enable building API documentation with Doxygen" OFF)
if(BUILD_DOCS)
    find_package(Doxygen QUIET)
    if(DOXYGEN_FOUND)
        set(DOXYGEN_INPUT_DIRS
            "${PROJECT_SOURCE_DIR}/src"
            "${PROJECT_SOURCE_DIR}/src/include"
            "${PROJECT_SOURCE_DIR}/core/src"
            "${PROJECT_SOURCE_DIR}/core/src/include"
            "${PROJECT_SOURCE_DIR}/docs"
        )
        # Doxygen expects space-separated paths, not semicolon-separated lists
        string(REPLACE ";" " " DOXYGEN_INPUT_DIRS_STR "${DOXYGEN_INPUT_DIRS}")

        # Configure Doxyfile from template
        set(DOXYFILE_IN "${PROJECT_SOURCE_DIR}/docs/Doxyfile.in")
        set(DOXYFILE_OUT "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile")
        set(PROJECT_VERSION "${PROJECT_VERSION}")
    configure_file(${DOXYFILE_IN} ${DOXYFILE_OUT} @ONLY)

        add_custom_target(docs
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE_OUT}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating API documentation with Doxygen"
            VERBATIM
        )

        add_custom_target(doc DEPENDS docs)
        message(STATUS "Doxygen found. 'docs' target is available.")
    else()
        message(WARNING "Doxygen not found. Set BUILD_DOCS=OFF or install Doxygen to build documentation.")
    endif()
endif()


