import hakopy
import sys
import random
import time
import os

def my_on_initialize(context):
    return 0

def my_on_reset(context):
    return 0

def my_on_simulation_step(context):
    #print(f"INFO: {my_asset_name} - step")
    perform_work()  # 所定の作業を呼び出す
    return 0

my_asset_name = None

# フラグファイルの作成
def create_flag_file(asset_name):
    flag_file = f"./measure_dir/{asset_name}.txt"
    with open(flag_file, "w") as f:
        f.write("Flag created.")
    print(f"INFO: {asset_name} - Flag file created: {flag_file}")

# フラグファイルの削除
def delete_flag_file(asset_name):
    flag_file = f"./measure_dir/{asset_name}.txt"
    if os.path.exists(flag_file):
        os.remove(flag_file)
        print(f"INFO: {asset_name} - Flag file deleted: {flag_file}")
    else:
        print(f"WARNING: {asset_name} - Flag file not found: {flag_file}")

perform_count = 0
end_flag = False
perform_max = 100
def perform_work():
    global perform_count
    global perform_max
    global end_flag
    if end_flag:
        time.sleep(0.1)
        return
    
    # 1000回から10,000回の間で乱数のビジーループを実行
    #busy_loop_count = random.randint(1000, 10000)
    busy_loop_count = 10000
    #print(f"busy loop num: {busy_loop_count}")
    for _ in range(busy_loop_count):
        pass  # ビジーループ (何もしない)

    perform_count += 1
    print(f"INFO: {my_asset_name} - Performing work count: {perform_count}")
    if perform_count > perform_max:
        end_flag = True
        delete_flag_file(my_asset_name)
        return True
    return False

my_callback = {
    'on_initialize': my_on_initialize,
    'on_simulation_step': my_on_simulation_step,
    'on_manual_timing_control': None,
    'on_reset': my_on_reset
}

def main():
    if len(sys.argv) != 5:
        print(f"Usage: {sys.argv[0]} <asset_name> <config_path> <delta_time_msec> <step_num>")
        return 1
    global my_asset_name
    global perform_max
    asset_name = sys.argv[1]
    config_path = sys.argv[2]
    delta_time_usec = int(sys.argv[3]) * 1000
    step_num = int(sys.argv[4])

    perform_max = step_num

    my_asset_name = asset_name
    # フラグファイルの作成
    create_flag_file(asset_name)

    ret = hakopy.asset_register(asset_name, config_path, my_callback, delta_time_usec, hakopy.HAKO_ASSET_MODEL_CONTROLLER)
    if not ret:
        print(f"ERROR: {asset_name} - hako_asset_register() returns {ret}.")
        return 1

    ret = hakopy.start()
    print(f"INFO: {asset_name} - hako_asset_start() returns {ret}")

    return 0

if __name__ == "__main__":
    sys.exit(main())
