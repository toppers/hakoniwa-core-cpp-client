import pandas as pd
import matplotlib.pyplot as plt
import argparse

# コマンドライン引数を設定
parser = argparse.ArgumentParser(description='Plot final results data')
parser.add_argument('--csv-file', type=str, required=True, help='Path to the CSV file containing the final results data')
parser.add_argument('--plot-type', choices=['elapse-vs-multi', 'stddev-vs-multi', 'stddev-vs-elapse'], required=True, help='Type of plot to generate')
parser.add_argument('--multi-num', type=int, help='Specify the multi-num (only for stddev-vs-elapse plot)')
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

# 1. 横軸：multi-num、縦軸：elapse_meanとして、max-delay毎に重ねて表示
if args.plot_type == 'elapse-vs-multi':
    plt.figure(figsize=(10, 6))
    for delay in sorted(df['max-delay'].unique()):
        subset = df[df['max-delay'] == delay].sort_values(by='multi-num')  # multi-numでソート
        plt.errorbar(subset['multi-num'], subset['elapse_mean'], yerr=subset['elapse_std'], marker='o', label=f'max-delay={delay}')
    plt.xlabel('multi-num')
    plt.ylabel('elapse mean (± std)')
    plt.title('Elapse Mean vs Multi-num with Stddev for different max-delay values')
    plt.legend()
    plt.grid(True)
    plt.tight_layout()

# 2. 横軸：multi-num、縦軸：stddev_meanとして、max-delay毎に重ねて表示
elif args.plot_type == 'stddev-vs-multi':
    plt.figure(figsize=(10, 6))
    for delay in sorted(df['max-delay'].unique()):
        subset = df[df['max-delay'] == delay].sort_values(by='multi-num')  # multi-numでソート
        plt.errorbar(subset['multi-num'], subset['stddev_mean'], yerr=subset['stddev_std'], marker='o', label=f'max-delay={delay}')
    plt.xlabel('multi-num')
    plt.ylabel('stddev mean (± std)')
    plt.title('Stddev Mean vs Multi-num with Stddev for different max-delay values')
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
    
    plt.xlabel('elapse mean (± std)')
    plt.ylabel('stddev mean (± std)')
    plt.title(f'Stddev Mean vs Elapse Mean for multi-num={args.multi_num}' if args.multi_num is not None else 'Stddev Mean vs Elapse Mean for different max-delay values')
    plt.legend()
    plt.grid(True)
    plt.tight_layout()

# グラフを表示
plt.show()
