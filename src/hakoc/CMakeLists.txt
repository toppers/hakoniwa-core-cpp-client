set(HAKO_CMAKE_VERSION ${HAKO_CMAKE_VERSION})

project(hako-client
    LANGUAGES C CXX
)

find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})



add_library(
    hakoc
    src/hako_client.cpp
)
set(OS_TYPE "posix")
if(WIN32)
    set(OS_TYPE "win")
endif(WIN32)
MESSAGE(STATUS "OS_TYPE=" ${OS_TYPE})

add_library(
    shakoc SHARED
    src/hako_capi.cpp
    src/hako_capi_py.cpp
    ${HAKO_CORE_SOURCE_DIR}/src/hako/hako_impl.cpp
    ${HAKO_CORE_SOURCE_DIR}/src/hako/hako_asset_impl.cpp
    ${HAKO_CORE_SOURCE_DIR}/src/hako/hako_simevent_impl.cpp
    ${HAKO_CORE_SOURCE_DIR}/src/hako/hako_master_impl.cpp
    ${HAKO_CORE_SOURCE_DIR}/src/hako/core/simulation/time/hako_time.cpp
    ${HAKO_CORE_SOURCE_DIR}/src/hako/core/rpc/hako_internal_rpc.cpp

    #for filelock && mmap
    #${HAKO_CORE_SOURCE_DIR}/src/hako/utils/hako_share/impl/posix/hako_sem_flock.cpp
    #${HAKO_CORE_SOURCE_DIR}/src/hako/utils/hako_share/impl/posix/hako_shared_memory_mmap.cpp
    #${HAKO_CORE_SOURCE_DIR}/src/hako/utils/hako_share/impl/${OS_TYPE}/hako_mmap.cpp
    #${HAKO_CORE_SOURCE_DIR}/src/hako/utils/hako_share/impl/${OS_TYPE}/hako_flock.cpp
    #${HAKO_CORE_SOURCE_DIR}/src/hako/utils/hako_share/impl/${OS_TYPE}/os_file_io.cpp

    #for sem && shared memory
    ${HAKO_CORE_SOURCE_DIR}/src/hako/utils/hako_share/impl/posix/hako_shared_memory_shm.cpp
    ${HAKO_CORE_SOURCE_DIR}/src/hako/utils/hako_share/impl/posix/hako_sem.cpp

    ${HAKO_CORE_SOURCE_DIR}/src/hako/utils/hako_string.cpp
    ${HAKO_CORE_SOURCE_DIR}/src/hako/utils/hako_logger.cpp
)


target_link_libraries(
    hakoc 
    hako
    -pthread
)

target_link_libraries(
    shakoc 
    -pthread
    ${PYTHON_LIBRARIES}
)

target_include_directories(
    hakoc
    PRIVATE /usr/local/include
    PRIVATE ${HAKO_CORE_SOURCE_DIR}/third-party/json/single_include
    PRIVATE ${HAKO_CORE_SOURCE_DIR}/src/include
)

target_include_directories(
    shakoc
    PRIVATE /usr/local/include
    PRIVATE ${HAKO_CORE_SOURCE_DIR}/third-party/json/single_include
    PRIVATE ${HAKO_CORE_SOURCE_DIR}/src/include
    PRIVATE ${HAKO_CORE_SOURCE_DIR}/src/hako
)
