cmake_minimum_required(VERSION 3.12)

project(hakoniwa_elixir_project
    LANGUAGES C CXX
)

# Erlangのインクルードディレクトリとライブラリディレクトリの設定
if(NOT DEFINED ERL_EI_INCLUDE_DIR)
    execute_process(
        COMMAND elixir --eval ":code.root_dir |> to_string() |> IO.puts"
        OUTPUT_VARIABLE ERLANG_PATH
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    if(NOT ERLANG_PATH)
        message(FATAL_ERROR "Could not find the Elixir installation. Check to see that 'elixir' is installed.")
    endif()
    set(ERL_EI_INCLUDE_DIR "${ERLANG_PATH}/usr/include")
    set(ERL_EI_LIBDIR "${ERLANG_PATH}/usr/lib")
endif()

include_directories(${ERL_EI_INCLUDE_DIR})
link_directories(${ERL_EI_LIBDIR})


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
    ei
    shakoc
    assets
)
# リンク設定
if(APPLE)
    set_target_properties(hako_asset_nif PROPERTIES
        SUFFIX ".so"
        LINK_FLAGS "-undefined dynamic_lookup -dynamiclib"
    )
else()
    set_target_properties(hako_asset_nif PROPERTIES
        SUFFIX ".so"
        LINK_FLAGS "-fPIC -shared"
    )
endif()
