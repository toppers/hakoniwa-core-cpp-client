set(HAKO_CMAKE_VERSION ${HAKO_CMAKE_VERSION})

project(hako-proxy
    LANGUAGES C CXX
)

include(FetchContent)
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz DOWNLOAD_EXTRACT_TIMESTAMP true)
FetchContent_MakeAvailable(json)

add_executable(
    hako-proxy
    src/hako_proxy.cpp
)

target_link_libraries(
    hako-proxy 
    hako
    -pthread
)

target_include_directories(
    hako-proxy 
    PRIVATE /usr/local/include
    PRIVATE ${HAKO_CORE_SOURCE_DIR}/src/include
    PRIVATE ${nlohmann_json_SOURCE_DIR}/single_include
)
