import csv
import sys
import os

def extract_max_delay_from_dir(directory_name):
    # ディレクトリ名から 'd<数値>' を抽出し、ミリ秒をマイクロ秒に変換
    try:
        delay_str = directory_name.split('-')[-1]  # 'd10'の部分を抽出
        if delay_str.startswith('d'):
            max_delay_msec = int(delay_str[1:])  # 'd10' -> 10
            max_delay_usec = max_delay_msec * 1000  # 10msec -> 10000usec
            return max_delay_usec
        else:
            raise ValueError("Invalid directory format. Expected 'd<number>' in directory name.")
    except (IndexError, ValueError) as e:
        print(f"Error extracting max delay: {e}")
        sys.exit(1)

def check_time_difference(file_path, max_delay):
    result = True
    # ファイルを開く
    with open(file_path, newline='') as csvfile:
        reader = csv.DictReader(csvfile)

        # ヘッダーの空白を取り除く
        reader.fieldnames = [name.strip() for name in reader.fieldnames]

        # 各行の差分を計算し、チェックする
        for row in reader:
            core_time = float(row['core-time'])
            asset_time = float(row['asset-time'])
            time_diff = abs(core_time - asset_time)

            # 最大許容遅延時間内かをチェック
            if time_diff <= max_delay:
                #print(f"OK: Core time: {core_time}, Asset time: {asset_time}, Difference: {time_diff}")
                pass
            else:
                result = False
                print(f"NG: Core time: {core_time}, Asset time: {asset_time}, Difference: {time_diff} (Exceeds max delay)")
    return result

if __name__ == "__main__":
    # コマンドライン引数からファイルパスを取得
    if len(sys.argv) != 2:
        print("Usage: python check_time_difference.py <csv_file>")
        sys.exit(1)

    file_path = sys.argv[1]

    # ディレクトリ名から最大許容遅延時間を抽出
    directory_name = os.path.basename(os.path.dirname(file_path))
    max_delay = extract_max_delay_from_dir(directory_name)

    print(f'file: {file_path} max_delay={max_delay}')

    # 時刻差分のチェックを実行
    ret = check_time_difference(file_path, max_delay)
    if ret:
        print("Passed")
