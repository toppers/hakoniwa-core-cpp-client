set(HAKO_CMAKE_VERSION ${HAKO_CMAKE_VERSION})

project(hako-client
    LANGUAGES C CXX
)

message(STATUS "CMAKE_VERSION:${CMAKE_VERSION}" )

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

# ベースとなるソースファイルリスト
set(SHARED_SRC
    src/hako_capi.cpp
    ${HAKO_CORE_SOURCE_DIR}/src/hako/hako_impl.cpp
    ${HAKO_CORE_SOURCE_DIR}/src/hako/hako_asset_impl.cpp
    ${HAKO_CORE_SOURCE_DIR}/src/hako/hako_simevent_impl.cpp
    ${HAKO_CORE_SOURCE_DIR}/src/hako/hako_master_impl.cpp
    ${HAKO_CORE_SOURCE_DIR}/src/hako/core/simulation/time/hako_time.cpp
    ${HAKO_CORE_SOURCE_DIR}/src/hako/core/rpc/hako_internal_rpc.cpp
    ${HAKO_CORE_SOURCE_DIR}/src/hako/utils/hako_share/impl/hako_sem_flock.cpp
    ${HAKO_CORE_SOURCE_DIR}/src/hako/utils/hako_share/impl/hako_shared_memory_mmap.cpp
    ${HAKO_CORE_SOURCE_DIR}/src/hako/utils/hako_share/impl/${OS_TYPE}/hako_mmap.cpp
    ${HAKO_CORE_SOURCE_DIR}/src/hako/utils/hako_share/impl/${OS_TYPE}/hako_flock.cpp
    ${HAKO_CORE_SOURCE_DIR}/src/hako/utils/hako_share/impl/hako_shared_memory_factory.cpp
    ${HAKO_CORE_SOURCE_DIR}/src/hako/utils/hako_share/impl/${OS_TYPE}/os_file_io.cpp
    ${HAKO_CORE_SOURCE_DIR}/src/hako/utils/hako_string.cpp
    ${HAKO_CORE_SOURCE_DIR}/src/hako/utils/hako_logger.cpp
)

# ENABLE_HAKO_TIME_MEASURE が有効な場合、追加する
if(ENABLE_HAKO_TIME_MEASURE)
    list(APPEND SHARED_SRC ${HAKO_MEASURE_SRC_DIR}/hako_asset_impl_measure.cpp)
endif()

add_library(
    shakoc SHARED
    ${SHARED_SRC}
)
# POSIX特有のファイルをWIN32ではない場合にのみ追加
if(NOT WIN32)
    target_sources(shakoc PRIVATE
        ${HAKO_CORE_SOURCE_DIR}/src/hako/utils/hako_share/impl/posix/hako_shared_memory_shm.cpp
        ${HAKO_CORE_SOURCE_DIR}/src/hako/utils/hako_share/impl/posix/hako_sem.cpp
    )
endif()
if(MSVC)
elseif(WIN32)
    target_link_libraries(shakoc stdc++)
endif()
add_library(
    hakoarun
    src/hako_asset_runner.cpp
)
if (NOT WIN32)
    set(PTHREAD_LIB -pthread)
else()
    set(PTHREAD_LIB "")
endif()

# 各ターゲットに適用
target_link_libraries(
    hakoc 
    hako
    ${PTHREAD_LIB}
)

target_link_libraries(
    shakoc 
    ${PTHREAD_LIB}
    ${HAKO_PYTHON_LIBS}
)

target_link_libraries(
    hakoarun 
    hako
    ${PTHREAD_LIB}
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
if(ENABLE_HAKO_TIME_MEASURE)
target_include_directories(
    shakoc
    PRIVATE ${HAKO_MEASURE_SRC_DIR}
)
endif()

target_include_directories(
    hakoarun
    PRIVATE /usr/local/include
    PRIVATE ${HAKO_CORE_SOURCE_DIR}/src/include
    PRIVATE ${nlohmann_json_SOURCE_DIR}/single_include
)
