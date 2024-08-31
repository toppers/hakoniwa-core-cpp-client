cmake_minimum_required(VERSION 3.12)

project(hakoniwa_elixir_project
    LANGUAGES C CXX
)

# Erlangのインクルードディレクトリとライブラリディレクトリを手動で設定
set(ERLANG_INCLUDE_DIR "/opt/homebrew/opt/erlang/lib/erlang/usr/include")
set(ERLANG_LIBRARY_DIR "/opt/homebrew/opt/erlang/lib")

# インクルードディレクトリを指定
include_directories(${ERLANG_INCLUDE_DIR})

# ライブラリディレクトリを指定
link_directories(${ERLANG_LIBRARY_DIR})

message(STATUS "CMAKE_VERSION: ${CMAKE_VERSION}")
message(STATUS "ERLANG_INCLUDE_DIR: ${ERLANG_INCLUDE_DIR}")
message(STATUS "ERLANG_LIBRARY_DIR: ${ERLANG_LIBRARY_DIR}")

# NIFをビルド
add_library(
    hako_asset_nif SHARED
    hako_asset_elixir_nif.c
)

# Erlangのライブラリとリンク
target_link_libraries(
    hako_asset_nif
    erl_interface
    ei
)
