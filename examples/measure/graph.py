import pandas as pd
import matplotlib.pyplot as plt
import argparse
import os

# コマンドライン引数を設定
parser = argparse.ArgumentParser(description='Measure data plotter')
parser.add_argument('--multi-num', type=int, required=True, help='Index number for the measure file')
parser.add_argument('--type', choices=['time', 'phase'], required=True, help='Select either "time" or "phase"')
parser.add_argument('--start', type=int, required=False, default=0, help='Start time for the data')
parser.add_argument('--duration', type=int, required=False, default=-1, help='Duration for the data')
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

    # startとdurationの範囲にデータをフィルタリング
    if args.duration > 0:
        end_time = args.start + args.duration
        df = df[(core_time >= args.start) & (core_time <= end_time)]
    else:
        df = df[core_time >= args.start]

    # フィルタ後の core_time, asset_timeを再計算
    asset_time = df['asset-time'] / 1000.0
    core_time = df['core-time'] / 1000.0

    if args.type == 'phase':
        x_value = asset_time
        y_value = core_time
        if i == 0:
            plt.plot(x_value, x_value, label=f'base', marker='o')
        plt.plot(x_value, y_value, label=f'asset-{i}', marker='o')
    elif args.type == 'time':
        x_value = core_time
        y_value = core_time - asset_time
        plt.plot(x_value, y_value, label=f'asset-{i}', marker='o')

if args.type == 'phase':
    plt.xlabel('core-time')
    plt.ylabel('asset-time')
elif args.type == 'time':
    plt.xlabel('core-time')
    plt.ylabel('core-time - asset-time')

plt.title('hako-time graph')
plt.legend()
plt.grid(True)
plt.tight_layout()

# すべてのアセットをプロットした後にグラフを表示
plt.show()
