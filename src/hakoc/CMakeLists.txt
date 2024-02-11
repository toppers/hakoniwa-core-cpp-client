set(HAKO_CMAKE_VERSION ${HAKO_CMAKE_VERSION})

project(hako-client
    LANGUAGES C CXX
)

if( ${CMAKE_VERSION} LESS 3.12 )
    find_package(PythonLibs REQUIRED)
    include_directories(${Python_INCLUDE_DIRS})
    set(HAKO_PYTHON_LIBS ${PYTHON_LIBRARIES})
else()
    find_package(Python COMPONENTS Interpreter Development)
    include_directories(${Python_INCLUDE_DIRS})
    set(HAKO_PYTHON_LIBS ${Python_LIBRARIES})
endif()

message(STATUS "CMAKE_VERSION:${CMAKE_VERSION}" )
message(STATUS "PYTHON_INCLUDE_DIRS:${PYTHON_INCLUDE_DIRS}" )
message(STATUS "Python_INCLUDE_DIRS:${Python_INCLUDE_DIRS}" )
message(STATUS "PYTHON_LIBRARIES:${PYTHON_LIBRARIES}" )
message(STATUS "Python_LIBRARIES:${Python_LIBRARIES}" )
message(STATUS "Python_LIBRARY_DIRS:${Python_LIBRARY_DIRS}" )

add_library(
    hakoc
    src/hako_client.cpp
)
set(OS_TYPE "posix")
if(WIN32)
    set(OS_TYPE "win")
endif(WIN32)
MESSAGE(STATUS "OS_TYPE=" ${OS_TYPE})

include(FetchContent)
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz DOWNLOAD_EXTRACT_TIMESTAMP true)
FetchContent_MakeAvailable(json)

add_library(
    shakoc SHARED
    src/hako_capi.cpp
    ${HAKO_CORE_SOURCE_DIR}/src/hako/hako_impl.cpp
    ${HAKO_CORE_SOURCE_DIR}/src/hako/hako_asset_impl.cpp
    ${HAKO_CORE_SOURCE_DIR}/src/hako/hako_simevent_impl.cpp
    ${HAKO_CORE_SOURCE_DIR}/src/hako/hako_master_impl.cpp
    ${HAKO_CORE_SOURCE_DIR}/src/hako/core/simulation/time/hako_time.cpp
    ${HAKO_CORE_SOURCE_DIR}/src/hako/core/rpc/hako_internal_rpc.cpp

    #for filelock && mmap
    ${HAKO_CORE_SOURCE_DIR}/src/hako/utils/hako_share/impl/hako_sem_flock.cpp
    ${HAKO_CORE_SOURCE_DIR}/src/hako/utils/hako_share/impl/hako_shared_memory_mmap.cpp
    ${HAKO_CORE_SOURCE_DIR}/src/hako/utils/hako_share/impl/${OS_TYPE}/hako_mmap.cpp
    ${HAKO_CORE_SOURCE_DIR}/src/hako/utils/hako_share/impl/${OS_TYPE}/hako_flock.cpp
    
    #for sem && shared memory
    ${HAKO_CORE_SOURCE_DIR}/src/hako/utils/hako_share/impl/hako_shared_memory_factory.cpp
    ${HAKO_CORE_SOURCE_DIR}/src/hako/utils/hako_share/impl/posix/hako_shared_memory_shm.cpp
    ${HAKO_CORE_SOURCE_DIR}/src/hako/utils/hako_share/impl/posix/hako_sem.cpp

    ${HAKO_CORE_SOURCE_DIR}/src/hako/utils/hako_string.cpp
    ${HAKO_CORE_SOURCE_DIR}/src/hako/utils/hako_logger.cpp
)
add_library(
    hakoarun
    src/hako_asset_runner.cpp
)

target_link_libraries(
    hakoc 
    hako
    -pthread
)

target_link_libraries(
    shakoc 
    -pthread
    ${HAKO_PYTHON_LIBS}
)
target_link_libraries(
    hakoarun 
    hako
    -pthread
)

target_include_directories(
    hakoc
    PRIVATE /usr/local/include
    PRIVATE ${HAKO_CORE_SOURCE_DIR}/src/include
    PRIVATE ${nlohmann_json_SOURCE_DIR}/single_include
)

target_include_directories(
    shakoc
    PRIVATE /usr/local/include
    PRIVATE ${HAKO_CORE_SOURCE_DIR}/src/include
    PRIVATE ${HAKO_CORE_SOURCE_DIR}/src/hako
    PRIVATE ${nlohmann_json_SOURCE_DIR}/single_include
)

target_include_directories(
    hakoarun
    PRIVATE /usr/local/include
    PRIVATE ${HAKO_CORE_SOURCE_DIR}/src/include
    PRIVATE ${nlohmann_json_SOURCE_DIR}/single_include
)
