#!/bin/bash

# Google Test のインストール (失敗時はエラーメッセージ表示)
if ! brew install googletest; then
  echo "Failed to install Google Test."
  exit 1
fi

# jq のインストール (失敗時はエラーメッセージ表示)
if ! brew install jq; then
  echo "Failed to install jq."
  exit 1
fi

