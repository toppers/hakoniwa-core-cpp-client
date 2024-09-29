#!/bin/bash

# Google Test のインストール (失敗時はエラーメッセージ表示)
if ! ${SUDO} apt install -y libgtest-dev; then
  echo "Failed to install Google Test."
  exit 1
fi

# jq のインストール (失敗時はエラーメッセージ表示)
if ! ${SUDO} apt install -y jq; then
  echo "Failed to install jq."
  exit 1
fi

# python3-dev のインストール (失敗時はエラーメッセージ表示)
if ! ${SUDO} apt install -y python3-dev; then
  echo "Failed to install python3-dev."
  exit 1
fi

if ! ${SUDO} apt install -y python3; then
  echo "Failed to install python3."
  exit 1
fi


if ! ${SUDO} apt install -y python3-pip; then
  echo "Failed to install python3-pip."
  exit 1
fi

if ! ${SUDO} apt install -y cmake; then
  echo "Failed to install cmake."
  exit 1
fi


