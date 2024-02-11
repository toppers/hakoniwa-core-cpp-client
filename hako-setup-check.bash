#!/bin/bash

STATUS="OK"

# 必要なディレクトリのリスト
directories=(
    "/usr/local/bin"
    "/usr/local/bin/hakoniwa"
    "/usr/local/lib"
    "/usr/local/lib/hakoniwa"
    "/etc/hakoniwa"
    "/var/lib/hakoniwa"
    "/var/lib/hakoniwa/mmap"
)
OS=`uname`
if [ "$OS" = "Linux" ]; then
    LIB_EXT=".so"
elif [ "$OS" = "Darwin" ]; then
    LIB_EXT=".dylib"
else
    LIB_EXT=".so"
fi

# OSに応じたファイルのリスト
files=(
    "/etc/hakoniwa/cpp_core_config.json"
    "/usr/local/bin/hakoniwa/hako-cmd"
    "/usr/local/lib/hakoniwa/libhakoarun.a"
    "/usr/local/lib/hakoniwa/libshakoc$LIB_EXT"
    "/usr/local/lib/hakoniwa/hakoc.so" # このファイル名は固定
    "/usr/local/lib/hakoniwa/libassets$LIB_EXT"
    "/usr/local/lib/hakoniwa/libconductor$LIB_EXT"
    "/usr/local/lib/hakoniwa/py" # ディレクトリのチェック
)

if [ "$(uname)" = "Linux" -o "$(uname)" = "Darwin" ]; then
    files+=("/usr/local/bin/hakoniwa/hako-proxy")
fi

# ディレクトリの存在をチェック
for dir in "${directories[@]}"; do
    if [ ! -d "$dir" ]; then
        echo "NG Missing directory: $dir"
        STATUS="NG"
    else
        echo "OK Directory exists: $dir"
    fi
done

# ファイルの存在をチェック
for file in "${files[@]}"; do
    if [ ! -e "$file" ]; then
        echo "NG Missing file: $file"
        STATUS="NG"
    else
        echo "OK File exists: $file"
    fi
done

if [ $STATUS = "OK" ]
then
    echo "Check complete."
    exit 0
else
    echo "Check failed."
    exit 1
fi
