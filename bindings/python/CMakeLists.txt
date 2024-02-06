cmake_minimum_required(VERSION 3.12)

project(hakoniwa_python_project
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
    hako_asset_python SHARED
    hako_asset_python.cpp
)

target_link_libraries(
    hako_asset_python 
    assets
    ${HAKO_PYTHON_LIBS}
)
