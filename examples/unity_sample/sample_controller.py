#!/usr/bin/python
# -*- coding: utf-8 -*-

import hakopy
import hako_pdu
import sys
from enum import Enum

pdu_manager = None
robot_controller = None

class RobotController:
    def __init__(self):
        global pdu_manager
        self.message = None
        self.cmd_vel_cid = 3
        self.touch_sensor_cid = 0
        self.do_move = True
        self.pdu_cmd_vel = pdu_manager.get_pdu('SampleRobot', self.cmd_vel_cid)
        self.pdu_touch_sensor = pdu_manager.get_pdu('SampleRobot', self.touch_sensor_cid)
        self.d_cmd_vel = self.pdu_cmd_vel.get()

    def do_read(self):
        self.d_touch_sensor = self.pdu_touch_sensor.read()
    
    def do_write(self):
        if self.do_move:
            self.d_cmd_vel['linear']['x'] = 0.5
        else:
            self.d_cmd_vel['linear']['x'] = 0.0
        self.pdu_cmd_vel.write()

    def run_init(self):
        #motor
        self.d_cmd_vel['linear']['x'] = 0
        self.d_cmd_vel['linear']['y'] = 0
        self.d_cmd_vel['linear']['z'] = 0
        self.pdu_cmd_vel.write()
        self.message = "START"

    def run(self):
        self.do_read()
        #print("touch_sensor: ", self.d_touch_sensor['data'])
        if self.d_touch_sensor['data']:
            if self.do_move == True:
                self.do_move = False
                self.message = "Collided"
        else:
            self.do_move = True
        self.do_write()

        if self.message is not None:
            print("INFO: ", self.message)
            self.message = None


def my_on_initialize(context):
    robot_controller.run_init()
    return 0

def my_on_reset(context):
    return 0

def my_on_simulation_step(context):
    robot_controller.run()
    return 0

my_callback = {
    'on_initialize': my_on_initialize,
    'on_simulation_step': my_on_simulation_step,
    'on_manual_timing_control': None,
    'on_reset': my_on_reset
}
def main():
    global pdu_manager
    global robot_controller
    if len(sys.argv) != 3:
        print(f"Usage: {sys.argv[0]} <config_path> <delay>")
        return 1

    asset_name = 'SampleRobotController'
    config_path = sys.argv[1]
    delay = int(sys.argv[2])
    delta_time_usec = 20000
    max_delay_time_usec = delta_time_usec + (delta_time_usec * delay)

    pdu_manager = hako_pdu.HakoPduManager('/usr/local/lib/hakoniwa/hako_binary/offset', config_path)
    hakopy.conductor_start(delta_time_usec, max_delay_time_usec)

    robot_controller = RobotController()
    ret = hakopy.asset_register(asset_name, config_path, my_callback, delta_time_usec, hakopy.HAKO_ASSET_MODEL_CONTROLLER)
    if ret == False:
        print(f"ERROR: hako_asset_register() returns {ret}.")
        return 1

    ret = hakopy.start()

    hakopy.conductor_stop()
    return 0

if __name__ == "__main__":
    sys.exit(main())
