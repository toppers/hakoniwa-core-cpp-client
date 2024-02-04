cmake_minimum_required(VERSION 3.12)
project(hakoniwa_multi_language_project)

find_package(SWIG REQUIRED)
include(${SWIG_USE_FILE})
set(CMAKE_SWIG_FLAGS "")

# ヘッダーファイルのインクルードパスを設定
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src/include)
# SWIGインターフェースファイルの相対パスを設定
set(SWIG_INTERFACE_FILE ${CMAKE_CURRENT_SOURCE_DIR}/hako_asset.i)
set_source_files_properties(${SWIG_INTERFACE_FILE} PROPERTIES CPLUSPLUS ON)
# ライブラリのフルパスを検索
find_library(ASSETS_LIBRARY
    NAMES assets
    PATHS /usr/local/lib/hakoniwa
)

add_subdirectory(python)
add_subdirectory(ruby)

