cmake_minimum_required(VERSION 3.12)
project(hakoniwa_python_swig_project)

find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_PATH})

swig_add_library(hako_asset_py LANGUAGE python SOURCES ${SWIG_INTERFACE_FILE})
swig_link_libraries(hako_asset_py ${PYTHON_LIBRARIES} ${ASSETS_LIBRARY})
