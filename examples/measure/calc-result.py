import os
import csv

# 結果を格納するリスト
results = []

# 検索するディレクトリのパス
base_dir = './results'

# ディレクトリ内の結果ファイルを探索
for folder_name in os.listdir(base_dir):
    folder_path = os.path.join(base_dir, folder_name)

    if os.path.isdir(folder_path):
        # ディレクトリ名から `m` と `d` の値を抽出
        parts = folder_name.split('-')
        if len(parts) == 3 and parts[1].startswith('m') and parts[2].startswith('d'):
            multi_num = parts[1][1:]  # 'm' の後の数値を取得
            max_delay = parts[2][1:]  # 'd' の後の数値を取得

            # elapse.txt と stddev.txt のファイルパス
            elapse_file = os.path.join(folder_path, 'elapse.txt')
            stddev_file = os.path.join(folder_path, 'stddev.txt')

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

            # 結果をリストに追加
            results.append([max_delay, multi_num, elapse_content, stddev_content])

# CSVファイルとして保存
csv_file = 'results.csv'
with open(csv_file, mode='w', newline='') as file:
    writer = csv.writer(file)
    # ヘッダーを書き込む
    writer.writerow(['max-delay', 'multi-num', 'elapse', 'stddev'])
    # データを書き込む
    writer.writerows(results)

print(f"結果が {csv_file} に保存されました。")
