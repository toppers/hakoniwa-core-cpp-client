import os
import pandas as pd
import numpy as np
import glob

# ディレクトリとファイルのパス設定
base_dir = './results'
output_file = 'final_results.csv'

# 全ての index ディレクトリを対象に results.csv を読み込む
all_data = []

for index_folder in os.listdir(base_dir):
    index_folder_path = os.path.join(base_dir, index_folder)
    results_file = os.path.join(index_folder_path, 'results.csv')
    
    if os.path.exists(results_file):
        # results.csv を読み込んでデータをリストに追加
        df = pd.read_csv(results_file)
        all_data.append(df)

# 全ての index ディレクトリからのデータを1つに統合
combined_df = pd.concat(all_data, ignore_index=True)

# max-delay と multi-num ごとにグループ化し、elapse, stddev, mean の平均と標準偏差を計算
grouped = combined_df.groupby(['max-delay', 'multi-num']).agg(
    elapse_mean=('elapse', 'mean'),
    elapse_std=('elapse', 'std'),
    stddev_mean=('stddev', 'mean'),
    stddev_std=('stddev', 'std'),
    mean_mean=('mean', 'mean'),
    mean_std=('mean', 'std')
).reset_index()

# 結果を CSV ファイルに保存
grouped.to_csv(output_file, index=False)

print(f"結果が {output_file} に保存されました。")
