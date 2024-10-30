cmake_minimum_required(VERSION 3.12)

project(hakoniwa_python_project
    LANGUAGES C CXX
)

if( ${CMAKE_VERSION} LESS 3.12 )
    find_package(PythonLibs REQUIRED)
    include_directories(${Python_INCLUDE_DIRS})
    set(HAKO_PYTHON_LIBS ${PYTHON_LIBRARIES})
else()
    # check pyenv
    execute_process(
        COMMAND pyenv which python
        RESULT_VARIABLE PYENV_FOUND
        OUTPUT_VARIABLE PYENV_PYTHON_PATH
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    message(STATUS "PYENV_FOUND:${PYENV_FOUND}")

    if(PYENV_FOUND EQUAL 0)
        # if pyenv found, use pyenv python
        set(Python_EXECUTABLE ${PYENV_PYTHON_PATH})
        set(Python_ROOT_DIR ${PYENV_PYTHON_PATH})
    endif()

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
