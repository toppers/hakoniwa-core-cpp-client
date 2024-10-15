import pandas as pd
import matplotlib.pyplot as plt
import argparse
import os
import numpy as np

# コマンドライン引数を設定
parser = argparse.ArgumentParser(description='Measure data plotter')
parser.add_argument('--multi-num', type=int, required=True, help='Index number for the measure file')
parser.add_argument('--type', choices=['time', 'phase'], required=True, help='Select either "time" or "phase"')
parser.add_argument('--start', type=int, required=False, default=0, help='Start time for the data')
parser.add_argument('--duration', type=int, required=False, default=-1, help='Duration for the data')
parser.add_argument('--csv-dir', type=str, required=False, default='./', help='Directory containing the CSV files')
parser.add_argument('--stddev-only', action='store_true', help='Output only the standard deviation mean')
parser.add_argument('--mean-only', action='store_true', help='Output only the mean value of core time - asset time')

args = parser.parse_args()

# グラフの初期化（複数のアセットを同じグラフにプロットするため）
if not args.stddev_only and not args.mean_only:
    plt.figure(figsize=(10, 6))

# 標準偏差と平均を記録するリスト
std_devs = []
means = []

for i in range(0, args.multi_num):
    print(f"index: {i}")
    file_pattern = f"asset-{i}-measure.csv"
    csv_file = os.path.join(args.csv_dir, file_pattern)  # 指定されたディレクトリからCSVファイルを探す

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

    if args.type == 'phase' and not args.stddev_only and not args.mean_only:
        x_value = asset_time
        y_value = core_time
        plt.plot(x_value, y_value, label=f'asset-{i}', marker='o')
        if i == args.multi_num - 1:
            print(f'{args.start} {end_time}')
            plt.plot([args.start, end_time], [args.start, end_time], label='base', color='red', linestyle='--', linewidth=2)
        plt.xlabel('Asset Time')  # x軸のラベル
        plt.ylabel('Core Time')   # y軸のラベル
    elif args.type == 'time':
        x_value = core_time
        y_value = core_time - asset_time
        if not args.stddev_only and not args.mean_only:
            plt.plot(x_value, y_value, label=f'asset-{i}', marker='o')
        
        # アセットごとの標準偏差と平均を計算して出力
        std_dev = np.std(y_value)
        mean_value = np.mean(y_value)
        std_devs.append(std_dev)
        means.append(mean_value)
        print(f"Asset-{i} の標準偏差: {std_dev:.4f}")
        print(f"Asset-{i} の平均値: {mean_value:.4f}")

# 全アセットの標準偏差の平均を計算して出力
if std_devs:
    mean_std_dev = np.mean(std_devs)
    variance_std_dev = np.var(std_devs)  # 標準偏差の分散
    max_std_dev = np.max(std_devs)
    min_std_dev = np.min(std_devs)
    range_std_dev = max_std_dev - min_std_dev  # 最大値と最小値の差

    if not args.stddev_only and not args.mean_only:
        print(f"全アセットの標準偏差の平均: {mean_std_dev:.4f}")
        print(f"全アセットの標準偏差の分散: {variance_std_dev:.4f}")
        print(f"標準偏差の最大値: {max_std_dev:.4f}")
        print(f"標準偏差の最小値: {min_std_dev:.4f}")
        print(f"標準偏差の最大値と最小値の差: {range_std_dev:.4f}")
    elif args.stddev_only:
        with open(os.path.join(args.csv_dir, 'stddev.txt'), 'w') as f:
            f.write(f"{mean_std_dev:.2f}\n")

# 全アセットの平均値の平均を計算して出力
if means:
    mean_of_means = np.mean(means)
    if not args.stddev_only and not args.mean_only:
        print(f"全アセットの平均値の平均: {mean_of_means:.4f}")
    elif args.mean_only:
        with open(os.path.join(args.csv_dir, 'mean.txt'), 'w') as f:
            f.write(f"{mean_of_means:.5f}\n")

# すべてのアセットをプロットした後にグラフを表示
if not args.stddev_only and not args.mean_only:
    plt.title('hako-time graph')
    plt.legend()
    plt.grid(True)
    plt.tight_layout()
    plt.show()
