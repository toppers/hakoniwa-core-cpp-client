import pandas as pd
import matplotlib.pyplot as plt
import argparse

# コマンドライン引数を設定
parser = argparse.ArgumentParser(description='Plot final results data')
parser.add_argument('--csv-file', type=str, required=True, help='Path to the CSV file containing the final results data')
parser.add_argument('--plot-type', choices=['elapse-vs-multi', 'stddev-vs-multi', 'stddev-vs-elapse', 'mean-vs-multi'], required=True, help='Type of plot to generate')
parser.add_argument('--multi-num', type=int, help='Specify the multi-num (only for stddev-vs-elapse plot)')
parser.add_argument('--out-file', type=str, help='Plot file name(PDF)')
args = parser.parse_args()

# CSVファイルを読み込む
df = pd.read_csv(args.csv_file)

# データの整形
df['max-delay'] = df['max-delay'].astype(int)
df['multi-num'] = df['multi-num'].astype(int)
df['elapse_mean'] = df['elapse_mean'].astype(float)
df['elapse_std'] = df['elapse_std'].astype(float)
df['stddev_mean'] = df['stddev_mean'].astype(float)
df['stddev_std'] = df['stddev_std'].astype(float)
df['mean_mean'] = df['mean_mean'].astype(float)
df['mean_std'] = df['mean_std'].astype(float)

# 1. 横軸：multi-num、縦軸：elapse_meanとして、max-delay毎に重ねて表示
if args.plot_type == 'elapse-vs-multi':
    plt.figure(figsize=(10, 6))
    for delay in sorted(df['max-delay'].unique()):
        subset = df[df['max-delay'] == delay].sort_values(by='multi-num')  # multi-numでソート
        plt.errorbar(subset['multi-num'], subset['elapse_mean'], yerr=subset['elapse_std'], marker='o', label=f'max-delay={delay}')
    plt.xlabel('Hakoniwa assets [num]')
    plt.ylabel('The average total processing time [s]')
    # plt.title('Elapse Mean vs Multi-num with Stddev for different max-delay values')
    plt.legend()
    plt.grid(True)
    plt.tight_layout()

# 2. 横軸：multi-num、縦軸：stddev_meanとして、max-delay毎に重ねて表示
elif args.plot_type == 'stddev-vs-multi':
    plt.figure(figsize=(10, 6))
    for delay in sorted(df['max-delay'].unique()):
        subset = df[df['max-delay'] == delay].sort_values(by='multi-num')  # multi-numでソート
        plt.errorbar(subset['multi-num'], subset['stddev_mean'], yerr=subset['stddev_std'], marker='o', label=f'max-delay={delay}')
    plt.xlabel('Hakoniwa assets [num]')
    plt.ylabel('The standard deviation of delay time')
    # plt.title('Stddev Mean vs Multi-num with Stddev for different max-delay values')
    plt.legend()
    plt.grid(True)
    plt.tight_layout()

# 3. 横軸：elapse_mean、縦軸：stddev_meanとして、max-delay毎にプロット表示
elif args.plot_type == 'stddev-vs-elapse':
    plt.figure(figsize=(10, 6))

    if args.multi_num is not None:
        # 指定された multi-num のデータのみを使用
        df = df[df['multi-num'] == args.multi_num]
    
    for delay in sorted(df['max-delay'].unique()):
        subset = df[df['max-delay'] == delay].sort_values(by='elapse_mean')  # elapse_meanでソート
        plt.errorbar(subset['elapse_mean'], subset['stddev_mean'], xerr=subset['elapse_std'], yerr=subset['stddev_std'], fmt='o', label=f'max-delay={delay}')
    
    plt.xlabel('The average total processing time [s]')
    plt.ylabel('The standard deviation of delay time')
    # plt.title(f'Stddev Mean vs Elapse Mean for multi-num={args.multi_num}' if args.multi_num is not None else 'Stddev Mean vs Elapse Mean for different max-delay values')
    plt.legend()
    plt.grid(True)
    plt.tight_layout()

# 4. 横軸：multi-num、縦軸：mean_meanとして、max-delay毎に重ねて表示
elif args.plot_type == 'mean-vs-multi':
    plt.figure(figsize=(10, 6))
    for delay in sorted(df['max-delay'].unique()):
        subset = df[df['max-delay'] == delay].sort_values(by='multi-num')  # multi-numでソート
        plt.errorbar(subset['multi-num'], subset['mean_mean'], yerr=subset['mean_std'], marker='o', label=f'max-delay={delay}')
    plt.xlabel('multi-num')
    plt.ylabel('mean (± std)')
    plt.title('Mean vs Multi-num with Stddev for different max-delay values')
    plt.legend()
    plt.grid(True)
    plt.tight_layout()


# グラフをベクタ画像形式で保存
if args.out_file:
    output_file = args.out_file
    plt.savefig(output_file, format='pdf')
else:
    # グラフを表示
    plt.show()
