import hakopy
import sys
import time
import os
import subprocess
import signal

# Global variable to hold the list of processes
processes = []

def create_measure_dir(measure_dir):
    if not os.path.exists(measure_dir):
        os.makedirs(measure_dir)

    # Remove all files under measure_dir
    for file in os.listdir(measure_dir):
        file_path = os.path.join(measure_dir, file)
        if os.path.isfile(file_path):
            os.remove(file_path)

def start_assets_in_parallel(multi_num, delta_time_usec):
    global processes
    for inx in range(multi_num):
        asset_name = f"measure-{inx}"
        config_path = f"./custom.json"  # Assuming a config file for each
        print("INFO: activate proc ", asset_name)
        process = subprocess.Popen(
            ["/bin/bash", "./measure-asset-run.bash", asset_name, config_path, str(delta_time_usec // 1000)]
        )
        processes.append(process)

def wait_for_flag_files(multi_num, measure_dir):
    print("INFO: wait for flag files: measure-<inx>.txt")
    while True:
        time.sleep(1)
        if all(os.path.exists(os.path.join(measure_dir, f"measure-{inx}.txt")) for inx in range(multi_num)):
            break

def wait_for_completion(multi_num, measure_dir):
    print("INFO: wait for completion")
    while True:
        time.sleep(1)
        if all(not os.path.exists(os.path.join(measure_dir, f"measure-{inx}.txt")) for inx in range(multi_num)):
            break

def calculate_total_time(start_time):
    end_time = time.time()
    total_time = end_time - start_time
    return total_time

def handle_sigint(signal_received, frame):
    print("INFO: SIGINT detected. Terminating processes...")
    global processes
    for process in processes:
        process.terminate()  # Sends SIGTERM to the process
    time.sleep(1)  # Allow some time for processes to terminate gracefully
    for process in processes:
        if process.poll() is None:  # Check if process is still running
            process.kill()  # Force kill if still running
    hakopy.conductor_stop()  # Stop conductor
    print("INFO: All processes terminated.")
    sys.exit(0)

def main():
    if len(sys.argv) != 4:
        print(f"Usage: {sys.argv[0]} <multi-num> <delta_time_msec> <max_delay_time_msec>")
        return 1

    multi_num = int(sys.argv[1])
    delta_time_usec = int(sys.argv[2]) * 1000
    max_delay_time_usec = int(sys.argv[3]) * 1000

    # Set up SIGINT handler
    signal.signal(signal.SIGINT, handle_sigint)

    # Conductor start
    hakopy.conductor_start(delta_time_usec, max_delay_time_usec)

    # Create measure directory and clean it
    measure_dir = './measure_dir'
    create_measure_dir(measure_dir)

    # Start asset_measure.py in parallel
    start_assets_in_parallel(multi_num, delta_time_usec)

    # Wait for flag file generation
    wait_for_flag_files(multi_num, measure_dir)

    # Start hako-cmd and measure time
    start_time = time.time()
    subprocess.run(["hako-cmd", "start"])

    # Wait for all asset completion (flag file deletion)
    wait_for_completion(multi_num, measure_dir)

    # Calculate and print total time
    total_time = calculate_total_time(start_time)
    print(f"Total processing time: {total_time:.2f} seconds")

    # Stop hako-cmd 
    subprocess.run(["hako-cmd", "stop"])
    time.sleep(3)
    subprocess.run(["hako-cmd", "reset"])

    # Conductor stop
    hakopy.conductor_stop()

    return 0

if __name__ == "__main__":
    sys.exit(main())
