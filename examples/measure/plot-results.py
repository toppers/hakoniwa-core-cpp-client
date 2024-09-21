import pandas as pd
import matplotlib.pyplot as plt
import argparse

# コマンドライン引数を設定
parser = argparse.ArgumentParser(description='Plot results data')
parser.add_argument('--csv-file', type=str, required=True, help='Path to the CSV file containing the data')
parser.add_argument('--plot-type', choices=['elapse-vs-multi', 'stddev-vs-multi', 'stddev-vs-elapse'], required=True, help='Type of plot to generate')
parser.add_argument('--multi-num', type=int, help='Specify the multi-num (only for stddev-vs-elapse plot)')
args = parser.parse_args()

# CSVファイルを読み込む
df = pd.read_csv(args.csv_file)

# データの整形
df['max-delay'] = df['max-delay'].astype(int)
df['multi-num'] = df['multi-num'].astype(int)
df['elapse'] = df['elapse'].astype(float)
df['stddev'] = df['stddev'].astype(float)

# 1. 横軸：multi-num、縦軸：elapseとして、max-delay毎に重ねて表示
if args.plot_type == 'elapse-vs-multi':
    plt.figure(figsize=(10, 6))
    for delay in sorted(df['max-delay'].unique()):
        subset = df[df['max-delay'] == delay].sort_values(by='multi-num')  # multi-numでソート
        plt.plot(subset['multi-num'], subset['elapse'], marker='o', label=f'max-delay={delay}')
    plt.xlabel('multi-num')
    plt.ylabel('elapse')
    plt.title('Elapse vs Multi-num for different max-delay values')
    plt.legend()
    plt.grid(True)
    plt.tight_layout()

# 2. 横軸：multi-num、縦軸：stddevとして、max-delay毎に重ねて表示
elif args.plot_type == 'stddev-vs-multi':
    plt.figure(figsize=(10, 6))
    for delay in sorted(df['max-delay'].unique()):
        subset = df[df['max-delay'] == delay].sort_values(by='multi-num')  # multi-numでソート
        plt.plot(subset['multi-num'], subset['stddev'], marker='o', label=f'max-delay={delay}')
    plt.xlabel('multi-num')
    plt.ylabel('stddev')
    plt.title('Stddev vs Multi-num for different max-delay values')
    plt.legend()
    plt.grid(True)
    plt.tight_layout()

# 3. 横軸：elapse、縦軸：stddevとして、max-delay毎にプロット表示
elif args.plot_type == 'stddev-vs-elapse':
    plt.figure(figsize=(10, 6))

    if args.multi_num is not None:
        # 指定された multi-num のデータのみを使用
        df = df[df['multi-num'] == args.multi_num]
    
    for delay in sorted(df['max-delay'].unique()):
        subset = df[df['max-delay'] == delay].sort_values(by='elapse')  # elapseでソート
        plt.scatter(subset['elapse'], subset['stddev'], label=f'max-delay={delay}')
    
    plt.xlabel('elapse')
    plt.ylabel('stddev')
    plt.title(f'Stddev vs Elapse for multi-num={args.multi_num}' if args.multi_num is not None else 'Stddev vs Elapse for different max-delay values')
    plt.legend()
    plt.grid(True)
    plt.tight_layout()

# グラフを表示
plt.show()
