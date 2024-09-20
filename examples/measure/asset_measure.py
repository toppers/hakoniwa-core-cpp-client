import hakopy
import sys
import time

def my_on_initialize(context):
    return 0

def my_on_reset(context):
    return 0

def my_on_simulation_step(context):
    print("step")
    time.sleep(1.0)
    return 0


my_callback = {
    'on_initialize': my_on_initialize,
    'on_simulation_step': my_on_simulation_step,
    'on_manual_timing_control': None,
    'on_reset': my_on_reset
}
def main():
    if len(sys.argv) != 4:
        print(f"Usage: {sys.argv[0]} <asset_name> <config_path> <delta_time_msec>")
        return 1

    asset_name = sys.argv[1]
    config_path = sys.argv[2]
    delta_time_usec = int(sys.argv[3]) * 1000

    ret = hakopy.asset_register(asset_name, config_path, my_callback, delta_time_usec, hakopy.HAKO_ASSET_MODEL_CONTROLLER)
    if ret == False:
        print(f"ERROR: hako_asset_register() returns {ret}.")
        return 1

    ret = hakopy.start()
    print(f"INFO: hako_asset_start() returns {ret}")

    return 0

if __name__ == "__main__":
    sys.exit(main())
