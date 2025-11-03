#!/usr/bin/env bash
set -euo pipefail

# Simple helper to build Doxygen docs via CMake
# Usage: ./docs.bash [build-dir]

BUILD_DIR=${1:-cmake-build}

# Check doxygen availability
if ! command -v doxygen >/dev/null 2>&1; then
  echo "[ERROR] doxygen が見つかりません。macOS の場合: brew install doxygen" >&2
  exit 1
fi

# Configure CMake with docs enabled
cmake -S . -B "${BUILD_DIR}" -DBUILD_DOCS=ON -DCMAKE_BUILD_TYPE=Release

# Build docs target
cmake --build "${BUILD_DIR}" --target docs -- -j

OUT_HTML="${BUILD_DIR}/docs/html/index.html"
if [ -f "${OUT_HTML}" ]; then
  echo "[INFO] ドキュメントを生成しました: ${OUT_HTML}"
  # On macOS, try to open in default browser
  if command -v open >/dev/null 2>&1; then
    open "${OUT_HTML}" || true
  fi
else
  echo "[WARN] 期待する出力が見つかりませんでした: ${OUT_HTML}" >&2
fi
