set(HAKO_CMAKE_VERSION ${HAKO_CMAKE_VERSION})

project(hako-proxy
    LANGUAGES C CXX
)

find_package(spdlog REQUIRED)

add_executable(
    hako-proxy
    src/hako_proxy.cpp
)

target_link_libraries(
    hako-proxy 
    hako
    -pthread
    spdlog::spdlog
)


target_include_directories(
    hako-proxy 
    PRIVATE /usr/local/include
    PRIVATE ${HAKO_CORE_SOURCE_DIR}/third-party/json/single_include
    PRIVATE ${HAKO_CORE_SOURCE_DIR}/src/include
)
