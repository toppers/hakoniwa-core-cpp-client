#!/usr/bin/python
# -*- coding: utf-8 -*-
import json
import sys
from binary import offset_map
from binary import binary_writer
from binary import binary_reader
import hakoc
import time
from enum import Enum

# HakoSimAssetEvent_None = 0
# HakoSimAssetEvent_Start = 1
# HakoSimAssetEvent_Stop = 2
# HakoSimAssetEvent_Reset = 3
# HakoSimAssetEvent_Error = 4
class HakoEvent(Enum):
    NONE = 0
    START = 1
    STOP = 2
    RESET = 3
    ERROR = 4

# HakoSim_Stopped = 0,
# HakoSim_Runnable,
# HakoSim_Running,
# HakoSim_Stopping,
# HakoSim_Resetting,
# HakoSim_Error,
# HakoSim_Terminated,
class HakoState(Enum):
    STOPPED = 0
    RUNNABLE = 1
    RUNNING = 2
    STOPPING = 3
    RESETTING = 4
    ERROR = 5
    TERMINATED = 6

class Hako:
    def __init__(self, offset_path):
        self.offmap = offset_map.create_offmap(offset_path)
        self.write_channels = {}
        self.write_buffers = {}
        self.write_types = {}
        self.read_channels = {}
        self.read_types = {}
        self.read_buffers = {}
        self.asset_time = 0
        hakoc.asset_init()
        
    def register(self, name):
        self.asset_name = name
        hakoc.asset_register(name)
    
    def create_pdu_channel(self, channel_id, pdu_size, typename):
        self.write_channels[channel_id] = pdu_size
        self.write_buffers[channel_id] = bytearray(pdu_size)
        self.write_types[channel_id] = typename
        hakoc.asset_create_pdu_channel(channel_id, pdu_size)

    def subscribe_pdu_channel(self, channel_id, pdu_size, typename):
        self.read_channels[channel_id] = pdu_size
        self.read_buffers[channel_id] = bytearray(pdu_size)
        self.read_types[channel_id] = typename
    
    def state(self):
        return hakoc.state()

    def wait_event(self):
        while True:
            current_ev = hakoc.asset_get_event()
            if current_ev == HakoEvent.NONE:
                time.sleep(0.01)
            elif current_ev == HakoEvent.START:
                hakoc.asset_start_feedback(self.asset_name, True)
                return True
            elif current_ev == HakoEvent.STOP:
                hakoc.asset_stop_feedback(self.asset_name, True)
                return True
            elif current_ev == HakoEvent.RESET:
                hakoc.asset_reset_feedback(self.asset_name, True)
                return True
            else:
                print("ERROR: unknown event:" + current_ev)
                return False

    def stop(self):
        hakoc.stop()
        self.wait_event(HakoEvent.STOP)
    
    def reset(self):
        hakoc.reset()
        self.wait_event(HakoEvent.RESET)
        self.asset_time = 0
    
    def start(self):
        hakoc.start()
        self.wait_event(HakoEvent.START)
    
    def read_pdu(self, channel_id):
        hakoc.asset_read_pdu(self.asset_name, channel_id, self.read_buffers[channel_id], self.read_channels[channel_id])
    
    def read_pdus(self):
        ret = {}
        for channel_id in self.read_channels:
            self.read_pdu(channel_id)
            typename = self.read_types[channel_id]
            binary_data = self.read_buffers[channel_id]
            ret[channel_id] = binary_reader.binary_read(self.offmap, typename, binary_data)
    
    def write_pdu(self, channel_id, pdu_json):
        binary_data = self.write_buffers[channel_id]
        typename = self.write_types[channel_id]
        pdu_data = binary_writer.binary_write(self.offmap, binary_data, pdu_json, typename)
        self.write_buffers[channel_id] = pdu_data

    def write_pdus(self):
        for channel_id in self.write_channels:
            hakoc.asset_write_pdu(self.asset_name, channel_id, self.write_buffers[channel_id], self.write_channels[channel_id])

    def execute(self):
        while True:
            result = hakoc.asset_notify_simtime(self.asset_name, self.asset_time)
            if result == False:
                time.sleep(0.01)
            elif self.state() != HakoState.RUNNING:
                time.sleep(0.01)
            elif hakoc.asset_is_pdu_created() == False:
                time.sleep(0.01)
            elif hakoc.asset_is_simulation_mode() == False:
                time.sleep(0.01)
            elif hakoc.asset_is_pdu_sync_mode() == False:
                self.write_pdus()
                time.sleep(0.01)
            else:
                self.write_pdus()
                state = self.read_pdus()
                return state


