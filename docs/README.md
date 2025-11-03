# hakoniwa-core-cpp-client: API ドキュメント (Doxygen)

このプロジェクトの C/C++ API ドキュメントを Doxygen で生成するための手順です。

## 必要要件
- Doxygen (macOS なら `brew install doxygen`)
- CMake 3.20 以上

## 生成方法（推奨: CMake ターゲット）

1. ビルドディレクトリを用意し、CMake 設定時にドキュメント機能を有効化します。

```sh
cmake -S . -B cmake-build -DBUILD_DOCS=ON -DCMAKE_BUILD_TYPE=Release
cmake --build cmake-build --target docs
```

2. 生成された HTML は以下に出力されます。

- `cmake-build/docs/html/index.html`

## ワンライナー（macOS）

プロジェクトルートで以下を実行します:

```sh
./docs.bash
```

初回は自動的に CMake 設定と `docs` ターゲットのビルドを行います。

## 直接 Doxygen を実行したい場合（上級者向け）

CMake が生成する `Doxyfile`（`cmake-build/Doxyfile`）を使って、直接実行も可能です。

```sh
doxygen cmake-build/Doxyfile
```

## act でローカル実行（CI/CD テスト）

GitHub Actions ワークフローをローカルで実行する場合（[act](https://github.com/nektos/act) が必要）:

```sh
# actの設定（初回のみ、Medium イメージを使用）
mkdir -p ~/Library/Application\ Support/act
echo '-P ubuntu-latest=catthehacker/ubuntu:act-latest' > ~/Library/Application\ Support/act/actrc

# ワークフローを実行して成果物をローカルに保存
act push \
  --container-architecture linux/amd64 \
  -W .github/workflows/doxygen.yml \
  -s GITHUB_TOKEN="$(gh auth token)" \
  --artifact-server-path ./build/artifacts \
  --job build-docs

# 生成されたドキュメントを確認
open ./artifacts/doxygen-html-*/index.html
```

### 出力内容
- **HTML**: `./artifacts/doxygen-html-*/` - ブラウザで閲覧可能なドキュメント
- **XML**: `./artifacts/doxygen-xml-*/` - 他ツールとの連携用（Sphinx、解析ツールなど）

---
補足: 出力先や入力ディレクトリは `docs/Doxyfile.in` と `CMakeLists.txt` の設定で調整できます。