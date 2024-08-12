set(HAKO_CMAKE_VERSION ${HAKO_CMAKE_VERSION})

project(hako-examples-athrill_sample
    LANGUAGES C CXX
)

message(STATUS "CMAKE_VERSION:${CMAKE_VERSION}" )

set(OS_TYPE "posix")
if(WIN32)
    set(OS_TYPE "win")
endif(WIN32)
MESSAGE(STATUS "OS_TYPE=" ${OS_TYPE})

add_executable(
    athrill_sample
    src/athrill_sample.c
)

target_include_directories(
    athrill_sample
    PRIVATE src/include
    PRIVATE /usr/local/include
    PRIVATE ${HAKO_INC_DIR}
)

target_link_libraries(
    athrill_sample
    assets
    conductor
)
if(NOT WIN32)
    find_package(Threads REQUIRED)
    target_link_libraries(
        athrill_sample
        Threads::Threads
    )
endif()
