import hakopy
import sys
import time
import os
import subprocess

def main():
    if len(sys.argv) != 4:
        print(f"Usage: {sys.argv[0]} <multi-num> <delta_time_msec> <max_delay_time_msec>")
        return 1

    multi_num = int(sys.argv[1])
    delta_time_usec = int(sys.argv[2]) * 1000
    max_delay_time_usec = int(sys.argv[3]) * 1000

    # Conductor start
    hakopy.conductor_start(delta_time_usec, max_delay_time_usec)

    # Create measure directory and clean it
    measure_dir = './measure_dir'
    if not os.path.exists(measure_dir):
        os.makedirs(measure_dir)

    # Remove all files under measure_dir
    for file in os.listdir(measure_dir):
        file_path = os.path.join(measure_dir, file)
        if os.path.isfile(file_path):
            os.remove(file_path)

    # Start asset_measure.py in parallel
    processes = []
    for inx in range(multi_num):
        asset_name = f"measure-{inx}"
        config_path = f"./config_{inx}.json"  # Assuming a config file for each
        process = subprocess.Popen(
            ["python3", "asset_measure.py", asset_name, config_path, str(delta_time_usec // 1000)]
        )
        processes.append(process)

    # Wait for flag file generation
    while True:
        time.sleep(1)
        if all(os.path.exists(os.path.join(measure_dir, f"measure-{inx}.txt")) for inx in range(multi_num)):
            break

    # Start hako-cmd and measure time
    start_time = time.time()
    subprocess.run(["hako-cmd", "start"])

    # Wait for all asset completion (flag file deletion)
    while True:
        time.sleep(1)
        if all(not os.path.exists(os.path.join(measure_dir, f"measure-{inx}.txt")) for inx in range(multi_num)):
            break

    # Calculate and print total time
    end_time = time.time()
    total_time = end_time - start_time
    print(f"Total processing time: {total_time:.2f} seconds")

    # Conductor stop
    hakopy.conductor_stop()

    return 0

if __name__ == "__main__":
    sys.exit(main())
