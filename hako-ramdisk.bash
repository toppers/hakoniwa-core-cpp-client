#!/bin/bash

show_usage() {
    echo "Usage:"
    echo "  $0 -c /path/to -s size   # Create a RAM disk with size in MB"
    echo "  $0 -d /path/to           # Delete a RAM disk"
    echo "  $0 -l                    # List created RAM disks"
    exit 1
}

# OSの判定
OS=$(uname)
case "$OS" in
    Linux) OS="Linux";;
    Darwin) OS="macOS";;
    *) echo "Unsupported OS: $OS"; exit 1;;
esac

# 引数がない場合は使用方法を表示
if [ $# -eq 0 ]; then
    show_usage
fi

create_ramdisk() {
    size_in_blocks=$((size * 2048))  # MBをブロックサイズに変換
    volume_name=$(basename "$path")
    case "$OS" in
        "macOS")
            diskutil erasevolume HFS+ "$volume_name" `hdiutil attach -nomount ram://$size_in_blocks`
            ln -s /Volumes/"$volume_name" "$path"
            ;;
        "Linux")
            mkdir -p "$path"
            mount -t tmpfs -o size=${size}m tmpfs "$path"
            ;;
    esac
}

remove_ramdisk() {
    case "$OS" in
        "macOS")
            volume_name=$(basename "$path_to_delete")
            diskutil eject /Volumes/"$volume_name"
            if [ -L "$path_to_delete" ]; then
                rm "$path_to_delete"
            fi
            ;;
        "Linux")
            umount "$path_to_delete"
            ;;
    esac
}
list_ramdisks_macos() {
    # diskutil listからディスク識別子を抽出する正しい方法
    diskutil list | grep -o '\b/dev/disk[0-9]\+\b' | while read disk_identifier; do
        # マウントポイントを取得
        mount_point=$(diskutil info "$disk_identifier" | grep "Mount Point" | awk -F: '{print $2}' | xargs)
        if [ ! -z "$mount_point" ]; then
            echo "$disk_identifier is mounted at $mount_point"
        else
            :
            #echo "$disk_identifier has no mount point."
        fi
    done
}

list_ramdisks() {
    echo "Listing RAM disks..."
    case "$OS" in
        "macOS")
            list_ramdisks_macos
            ;;
        "Linux")
            grep "tmpfs" /proc/mounts | grep -v "/run" | grep -v "/sys" | grep -v "/dev"
            ;;
    esac
}

# コマンドラインオプションの解析
while getopts ":c:d:s:l" opt; do
  case $opt in
    c)
      action="create"
      path=$OPTARG
      ;;
    d)
      action="delete"
      path_to_delete=$OPTARG
      ;;
    s)
      size=$OPTARG
      ;;
    l)
      action="list"
      ;;
    \? | :)
      show_usage
      ;;
  esac
done

# 最小サイズ（MB）
MIN_SIZE=64

# サイズが最小サイズ未満の場合はエラー
if [ "$action" = "create" ] && [ "$size" -lt "$MIN_SIZE" ]; then
    echo "Error: Size must be at least $MIN_SIZE MB."
    exit 1
fi

# アクションに応じた処理
case "$action" in
    "create")
        if [ -z "$path" ] || [ -z "$size" ]; then
            echo "Both path and size must be specified for creation."
            exit 1
        else
            create_ramdisk
        fi
        ;;
    "delete")
        if [ -z "$path_to_delete" ]; then
            echo "Path must be specified for deletion."
            exit 1
        else
            remove_ramdisk
        fi
        ;;
    "list")
        list_ramdisks
        ;;
    *)
        show_usage
        ;;
esac

echo "Operation completed."
