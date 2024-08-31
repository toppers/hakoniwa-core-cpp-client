cmake_minimum_required(VERSION 3.12)
project(hakoniwa_multi_language_project)


include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src/include)
set(ASSETS_LIBRARY assets)

add_subdirectory(python)
#add_subdirectory(elixir)
#add_subdirectory(ruby)

