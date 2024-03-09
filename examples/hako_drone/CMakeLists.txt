set(HAKO_CMAKE_VERSION ${HAKO_CMAKE_VERSION})

project(hako-examples-drone
    LANGUAGES C CXX
)

message(STATUS "CMAKE_VERSION:${CMAKE_VERSION}" )

set(OS_TYPE "posix")
if(WIN32)
    set(OS_TYPE "win")
endif(WIN32)
MESSAGE(STATUS "OS_TYPE=" ${OS_TYPE})

add_executable(
    drone-cmd
    src/drone_cmd.c
)

target_include_directories(
    drone-cmd
    PRIVATE /usr/local/include
    PRIVATE src
    PRIVATE ${HAKO_INC_DIR}
)

target_link_libraries(
    drone-cmd
    assets
)