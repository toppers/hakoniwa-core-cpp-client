import pandas as pd
import matplotlib.pyplot as plt
import argparse
import os

# コマンドライン引数を設定
parser = argparse.ArgumentParser(description='Measure data plotter')
parser.add_argument('--multi-num', type=int, required=True, help='Index number for the measure file')
args = parser.parse_args()

# グラフの初期化（複数のアセットを同じグラフにプロットするため）
plt.figure(figsize=(10, 6))

for i in range(0, args.multi_num):
    print(f"index: {i}")
    file_pattern = f"asset-{i}-measure.csv"
    csv_file = os.path.join(os.getcwd(), file_pattern)

    # ファイルの存在確認
    if not os.path.exists(csv_file):
        print(f"ファイル {file_pattern} が見つかりませんでした。")
        continue  # 次のファイルに進む

    # CSVファイルを読み込む
    df = pd.read_csv(csv_file)
    # カラム名の前後にある空白を削除
    df.columns = df.columns.str.strip()

    # 必要な列が存在するか確認
    required_columns = ['core-time', 'asset-time']
    missing_columns = [col for col in required_columns if col not in df.columns]
    
    if missing_columns:
        print(f"ファイル {file_pattern} に必要な列 {missing_columns} が存在しません。")
        continue  # 次のファイルに進む

    asset_time = df['asset-time'] / 1000.0
    core_time = df['core-time'] / 1000.0
    x_value = asset_time
    y_value = core_time

    if i == 0:
        plt.plot(x_value, x_value, label=f'base', marker='o')
    # 各アセットのデータを同じグラフにプロット
    plt.plot(x_value, y_value, label=f'asset-{i}', marker='o')

# グラフのラベルやタイトルを設定
plt.xlabel('core-time')
plt.ylabel('asset-time')
plt.title('hako-time graph')
plt.legend()
plt.grid(True)
plt.tight_layout()

# すべてのアセットをプロットした後にグラフを表示
plt.show()
