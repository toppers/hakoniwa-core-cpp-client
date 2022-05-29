set(HAKO_CMAKE_VERSION ${HAKO_CMAKE_VERSION})

project(hako-client
    LANGUAGES C CXX
)

find_package(spdlog REQUIRED)

add_library(
    hakoc
    src/hako_client.cpp
)

target_link_libraries(
    hakoc 
    hako
    -pthread
    spdlog::spdlog
)

target_include_directories(
    hakoc
    PRIVATE /usr/local/include
    PRIVATE ${HAKO_CORE_SOURCE_DIR}/third-party/json/single_include
    PRIVATE ${HAKO_CORE_SOURCE_DIR}/src/include
)
