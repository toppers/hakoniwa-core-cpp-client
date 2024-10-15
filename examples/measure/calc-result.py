import os
import csv
import argparse

# コマンドライン引数を設定
parser = argparse.ArgumentParser(description='Process results data.')
parser.add_argument('--index', type=str, required=True, help='Index folder name to process (e.g., 0 or 1)')
args = parser.parse_args()

# 結果を格納するリスト
results = []

# 検索するディレクトリのパス
base_dir = './results'

# 指定されたindexフォルダ内のサブフォルダを探索
index_folder_path = os.path.join(base_dir, args.index)

if os.path.isdir(index_folder_path):
    for folder_name in os.listdir(index_folder_path):
        folder_path = os.path.join(index_folder_path, folder_name)

        if os.path.isdir(folder_path):
            # ディレクトリ名から `m` と `d` の値を抽出
            parts = folder_name.split('-')
            if len(parts) == 3 and parts[1].startswith('m') and parts[2].startswith('d'):
                multi_num = parts[1][1:]  # 'm' の後の数値を取得
                max_delay = parts[2][1:]  # 'd' の後の数値を取得

                # elapse.txt, stddev.txt, mean.txt のファイルパス
                elapse_file = os.path.join(folder_path, 'elapse.txt')
                stddev_file = os.path.join(folder_path, 'stddev.txt')
                mean_file = os.path.join(folder_path, 'mean.txt')

                # elapse.txt の内容を読み取る
                if os.path.exists(elapse_file):
                    with open(elapse_file, 'r') as ef:
                        elapse_content = ef.read().strip()
                else:
                    elapse_content = 'N/A'

                # stddev.txt の内容を読み取る
                if os.path.exists(stddev_file):
                    with open(stddev_file, 'r') as sf:
                        stddev_content = sf.read().strip()
                else:
                    stddev_content = 'N/A'

                # mean.txt の内容を読み取る
                if os.path.exists(mean_file):
                    with open(mean_file, 'r') as mf:
                        mean_content = mf.read().strip()
                else:
                    mean_content = 'N/A'

                # 結果をリストに追加
                results.append([max_delay, multi_num, elapse_content, stddev_content, mean_content])

# CSVファイルとして保存
csv_file = f'results/{args.index}/results.csv'
with open(csv_file, mode='w', newline='') as file:
    writer = csv.writer(file)
    # ヘッダーを書き込む
    writer.writerow(['max-delay', 'multi-num', 'elapse', 'stddev', 'mean'])
    # データを書き込む
    writer.writerows(results)

print(f"結果が {csv_file} に保存されました。")
