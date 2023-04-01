#!/usr/bin/python
# -*- coding: utf-8 -*-
import json
import sys
from hako_binary import offset_map
from hako_binary import binary_writer
from hako_binary import binary_reader
import hakoc
import time
import hako_robomodel
from enum import Enum

# HakoSimAssetEvent_None = 0
# HakoSimAssetEvent_Start = 1
# HakoSimAssetEvent_Stop = 2
# HakoSimAssetEvent_Reset = 3
# HakoSimAssetEvent_Error = 4
HakoEvent = {
    'NONE': 0,
    'START': 1,
    'STOP': 2,
    'RESET': 3,
    'ERROR': 4
}

# HakoSim_Stopped = 0,
# HakoSim_Runnable,
# HakoSim_Running,
# HakoSim_Stopping,
# HakoSim_Resetting,
# HakoSim_Error,
# HakoSim_Terminated,
HakoState = {
    'STOPPED': 0,
    'RUNNABLE': 1,
    'RUNNING': 2,
    'STOPPING': 3,
    'RESETTING': 4,
    'ERROR': 5,
    'TERMINATED': 6
}

class Hako:
    def __init__(self, offset_path, robo_type):
        self.offmap = offset_map.create_offmap(offset_path)
        self.write_pdusize = {}
        self.write_buffers = {}
        self.write_types = {}
        self.read_pdusize = {}
        self.read_types = {}
        self.read_buffers = {}
        self.asset_time_usec = 0
        self.robo_type = robo_type
        hakoc.asset_init()
    
    def register(self, name, control_asset_name, model_filepath = None):
        self.asset_name = name
        self.control_asset_name = control_asset_name
        self.robo = hako_robomodel.create(self, self.robo_type, model_filepath)
        hakoc.asset_register(name)
    
    def create_pdu_lchannel(self, channel_id, pdu_size, typename):
        self.write_pdusize[channel_id] = pdu_size
        self.write_buffers[channel_id] = bytearray(pdu_size)
        self.write_types[channel_id] = typename
        #print("create_channel: " + str(self.write_buffers[channel_id]))
        hakoc.asset_create_pdu_lchannel(self.control_asset_name, channel_id, pdu_size)

    def subscribe_pdu_lchannel(self, channel_id, pdu_size, typename):
        self.read_pdusize[channel_id] = pdu_size
        self.read_buffers[channel_id] = bytearray(pdu_size)
        self.read_types[channel_id] = typename
    
    def state(self):
        return hakoc.state()

    def wait_event(self, ev):
        while True:
            current_ev = hakoc.asset_get_event(self.asset_name)
            if current_ev == HakoEvent['NONE']:
                time.sleep(0.01)
            elif current_ev == HakoEvent['START']:
                hakoc.asset_start_feedback(self.asset_name, True)
                return True
            elif current_ev == HakoEvent['STOP']:
                hakoc.asset_stop_feedback(self.asset_name, True)
                return True
            elif current_ev == HakoEvent['RESET']:
                hakoc.asset_reset_feedback(self.asset_name, True)
                return True
            else:
                print("ERROR: unknown event:" + current_ev)
                return False

    def wait_state(self, expect_state):
        while True:
            if self.state() != expect_state:
                time.sleep(0.01)
            else:
                return True

    def stop(self):
        if self.state() == HakoState['RUNNING']:
            hakoc.stop()
            self.wait_event(HakoEvent['STOP'])
        self.wait_state(HakoState['STOPPED'])
    
    def reset(self):
        if self.state() == HakoState['STOPPED']:
            hakoc.reset()
            self.wait_event(HakoEvent['RESET'])
        self.asset_time_usec = 0
        self.wait_state(HakoState['STOPPED'])
    
    def start(self):
        if self.state() == HakoState['STOPPED']:
            hakoc.start()
            self.wait_event(HakoEvent['START'])
        self.wait_state(HakoState['RUNNING'])
    
    def read_pdu(self, channel_id):
        #print("hakoc.read_pdu: start")
        hakoc.asset_read_pdu(self.asset_name, self.control_asset_name, channel_id, self.read_buffers[channel_id], self.read_pdusize[channel_id])
        #print("hakoc.read_pdu: end")
        pass
    
    def read_pdus(self):
        ret = {}
        for channel_id in self.read_pdusize:
            self.read_pdu(channel_id)
            typename = self.read_types[channel_id]
            binary_data = self.read_buffers[channel_id]
            #print("read_pdu: channel_id=" + str(channel_id))
            ret[channel_id] = binary_reader.binary_read(self.offmap, typename, binary_data)
            #print("read_pdu done: channel_id=" + str(channel_id))
        hakoc.asset_notify_read_pdu_done(self.asset_name)
        return ret
    
    def write_pdu(self, channel_id, pdu_json):
        typename = self.write_types[channel_id]
        #print(json.dumps(pdu_json))
        binary_data = self.write_buffers[channel_id]
        binary_writer.binary_write(self.offmap, binary_data, pdu_json, typename)
        #i = 0
        #for b in binary_data:
        #    print("data[" + str(i) + "] = " + str(b))
        #    if i > 512:
        #        break
        #    i = i + 1
        #sys.exit(0)

    def write_pdus(self):
        for channel_id in self.write_pdusize:
            #print("wirte_pdus: asset_name:" + self.asset_name)
            #print("wirte_pdus: channel_id:" + str(channel_id))
            #print("wirte_pdus: write_pdusize:" + str(self.write_pdusize[channel_id]))        
            #print("wirte_pdus: write_buffers:" + str(self.write_buffers[channel_id]))
            hakoc.asset_write_pdu(self.asset_name, self.control_asset_name, channel_id, self.write_buffers[channel_id], self.write_pdusize[channel_id])
            hakoc.asset_notify_write_pdu_done(self.asset_name)

    def execute(self):
        while True:
            result = hakoc.asset_notify_simtime(self.asset_name, self.asset_time_usec)
            if result == False:
                print("notify_simtime: false")
                time.sleep(0.01)
            elif self.state() != HakoState['RUNNING']:
                print("running: false")
                time.sleep(0.01)
            elif hakoc.asset_is_pdu_created() == False:
                print("pdu_created: false")
                time.sleep(0.01)
            elif hakoc.asset_is_pdu_sync_mode(self.asset_name) == True:
                print("sync_mode: true")
                self.write_pdus()
                time.sleep(0.01)
            elif hakoc.asset_is_simulation_mode() == False:
                print("simulation mode: false")
                time.sleep(0.01)
            elif self.asset_time_usec >= hakoc.asset_get_worldtime():
                #print("asset_time_usec: false")
                time.sleep(0.01)
            else:
                #print("time: " + str(self.asset_time_usec))
                self.write_pdus()
                #print("write_pdu: done")
                state = self.read_pdus()
                #print("read_pdu: done")
                self.asset_time_usec = self.asset_time_usec + self.robo.delta_usec()
                time.sleep(0.01)
                return state


