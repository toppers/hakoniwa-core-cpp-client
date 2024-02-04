cmake_minimum_required(VERSION 3.12)
project(hakoniwa_ruby_swig_project)

find_package(Ruby REQUIRED)
include_directories(${RUBY_INCLUDE_DIRS})


# Ruby向けのSWIGライブラリの設定
swig_add_library(hako_asset_ruby LANGUAGE ruby SOURCES ${SWIG_INTERFACE_FILE})
swig_link_libraries(hako_asset_ruby ${RUBY_LIBRARY} ${ASSETS_LIBRARY})

