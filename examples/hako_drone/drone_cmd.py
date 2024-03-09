#!/usr/bin/python
# -*- coding: utf-8 -*-

import hakopy
import hako_pdu
import pdu_info
import sys
import time

pdu_manager = None
def do_takeoff():
    global pdu_manager
    command = pdu_manager.get_pdu('DroneFlightController', pdu_info.HAKO_AVATOR_CHANNEL_ID_CMD_TAKEOFF)
    pdu_cmd = command.get()
    pdu_cmd['header']['request'] = 1
    pdu_cmd['header']['result'] = 0
    pdu_cmd['header']['result_code'] = 0
    pdu_cmd['height'] = 5
    pdu_cmd['speed'] = 5
    ret = command.write()
    if ret == False:
        print('"ERROR: hako_asset_pdu_write')
        return 0
    
    while True:
        pdu = command.read()
        if pdu == None:
            print('ERROR: hako_asset_pdu_read')
            return 0
        if pdu['header']['result'] == 1:
            pdu['header']['result'] = 0
            command.write()
            print('DONE')
            break
    print("INFO: do_takeoff exit")
    return 0


def do_move(x, y):
    global pdu_manager
    command = pdu_manager.get_pdu('DroneFlightController', pdu_info.HAKO_AVATOR_CHANNEL_ID_CMD_MOVE)
    pdu_cmd = command.get()
    pdu_cmd['header']['request'] = 1
    pdu_cmd['header']['result'] = 0
    pdu_cmd['header']['result_code'] = 0
    pdu_cmd['x'] = x
    pdu_cmd['y'] = y
    pdu_cmd['speed'] = 5
    ret = command.write()
    if ret == False:
        print('"ERROR: hako_asset_pdu_write')
        return 0
    
    while True:
        pdu = command.read()
        if pdu == None:
            print('ERROR: hako_asset_pdu_read')
            return 0
        if pdu['header']['result'] == 1:
            pdu['header']['result'] = 0
            command.write()
            print('DONE')
            break
    print("INFO: do_move exit")
    return 0



def do_land():
    global pdu_manager
    command = pdu_manager.get_pdu('DroneFlightController', pdu_info.HAKO_AVATOR_CHANNEL_ID_CMD_LAND)
    pdu_cmd = command.get()
    pdu_cmd['header']['request'] = 1
    pdu_cmd['header']['result'] = 0
    pdu_cmd['header']['result_code'] = 0
    pdu_cmd['height'] = 0
    pdu_cmd['speed'] = 5
    ret = command.write()
    if ret == False:
        print('"ERROR: hako_asset_pdu_write')
        return 0
    
    while True:
        pdu = command.read()
        if pdu == None:
            print('ERROR: hako_asset_pdu_read')
            return 0
        if pdu['header']['result'] == 1:
            pdu['header']['result'] = 0
            command.write()
            print('DONE')
            break
    print("INFO: do_land exit")
    return 0


def main():
    global pdu_manager
    if len(sys.argv) != 2:
        print(f"Usage: {sys.argv[0]} <config_path>")
        return 1

    config_path = sys.argv[1]

    pdu_manager = hako_pdu.HakoPduManager('/usr/local/lib/hakoniwa/hako_binary/offset', config_path)
    ret = hakopy.init_for_external()
    if ret == False:
        print(f"ERROR: init_for_external() returns {ret}.")
        return 1
    do_takeoff()
    do_move(0, 5)
    do_land()
    return 0

if __name__ == "__main__":
    sys.exit(main())
