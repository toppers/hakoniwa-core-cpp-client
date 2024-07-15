#!/bin/bash

STATUS="OK"

OS=`uname`
if [ "$OS" = "Linux" ]; then
    OS_TYPE="posix"
    LIB_EXT=".so"
    LIB_PRE="lib"
elif [ "$OS" = "Darwin" ]; then
    OS_TYPE="posix"
    LIB_EXT=".dylib"
    LIB_PRE="lib"
else
    OS_TYPE="win"
    LIB_EXT=".lib"
    LIB_PRE=""
fi

# デフォルトの箱庭環境変数
if [ -z "$HAKO_CORE_PREFIX" ]; then
    if [ "$OS_TYPE" = "posix" ]; then
        HAKO_CORE_PREFIX="/usr/local"
    else
        HAKO_CORE_PREFIX="../local"
    fi
fi

if [ -z "$HAKO_CONFIG_PATH" ]; then
    if [ "$OS_TYPE" = "posix" ]; then
        HAKO_CONFIG_PREFIX="/etc/hakoniwa"
    else
        HAKO_CONFIG_PREFIX="../hakoniwa"
    fi
    HAKO_CONFIG_PATH="$HAKO_CONFIG_PREFIX/cpp_core_config.json"
fi

if [ -z "$HAKO_CORE_MMAP_PATH" ]; then
    if [ "$OS_TYPE" = "posix" ]; then
        HAKO_CORE_MMAP_PREFIX="/var/lib/hakoniwa"
    else
        HAKO_CORE_MMAP_PREFIX="/z"
    fi
    HAKO_CORE_MMAP_PATH="$HAKO_CORE_MMAP_PREFIX/mmap"
fi

#
echo "=== HAKO ENV ==="
echo "OS=$OS , OS_TYPE=$OS_TYPE"
echo "HAKO_CORE_PREFIX=$HAKO_CORE_PREFIX"
echo "HAKO_CONFIG_PREFIX=$HAKO_CONFIG_PREFIX"
echo "HAKO_CORE_MMAP_PREFIX=$HAKO_CORE_MMAP_PREFIX"
echo "HAKO_CONFIGNFIG_PATH=$HAKO_CONFIG_PATH"
echo "HAKO_CORE_MMAP_PATH=$HAKO_CORE_MMAP_PATH"
echo ""

# 必要なディレクトリのリスト
directories=(
    "$HAKO_CORE_PREFIX/bin"
    "$HAKO_CORE_PREFIX/bin/hakoniwa"
    "$HAKO_CORE_PREFIX/lib"
    "$HAKO_CORE_PREFIX/lib/hakoniwa"
    "$HAKO_CONFIG_PREFIX"
    "$HAKO_CORE_MMAP_PREFIX"
    "$HAKO_CORE_MMAP_PREFIX/mmap"
)

# OSに応じたファイルのリスト
files=(
    "$HAKO_CONFIG_PATH"
    "$HAKO_CORE_PREFIX/bin/hakoniwa/hako-cmd"
    "$HAKO_CORE_PREFIX/lib/hakoniwa/${LIB_PRE}shakoc$LIB_EXT"
    "$HAKO_CORE_PREFIX/lib/hakoniwa/${LIB_PRE}assets$LIB_EXT"
    "$HAKO_CORE_PREFIX/lib/hakoniwa/${LIB_PRE}conductor$LIB_EXT"
    "$HAKO_CORE_PREFIX/lib/hakoniwa/py" # ディレクトリのチェック
)

if [ "$(uname)" = "Linux" -o "$(uname)" = "Darwin" ]; then
    files+=("$HAKO_CORE_PREFIX/bin/hakoniwa/hako-proxy")
    files+=("$HAKO_CORE_PREFIX/lib/hakoniwa/hakoc.so") # このファイル名は固定
    files+=("$HAKO_CORE_PREFIX/lib/hakoniwa/${LIB_PRE}hakoarun.a")
else    # Windows
    files+=("$HAKO_CORE_PREFIX/lib/hakoniwa/hakoc.lib") # このファイル名は固定
    files+=("$HAKO_CORE_PREFIX/lib/hakoniwa/${LIB_PRE}hakoarun.lib")
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
