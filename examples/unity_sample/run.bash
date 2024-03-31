#!/bin/bash

cleanup() {
  echo "SIGINT received, stopping the background process..."
  kill -9 $bg_pid
}

trap cleanup SIGINT

python3.12 asset_controller.py ./custom.json &
bg_pid=$!

wait $bg_pid
