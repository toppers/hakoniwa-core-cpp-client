#!/usr/bin/python
# -*- coding: utf-8 -*-
import json
import sys
from binary import offset_map
from binary import binary_writer
from binary import binary_reader
import hako
import time


class HakoEnv:
    def __init__(self, model_name):
        offset_path = '/usr/local/lib/hakoniwa/binary/offset'
        self.hako = hako.Hako(offset_path)
        #TODO robot dependent code..
        self.hako.create_pdu_channel(0, 1024, 'Ev3PduActuator')
        #TODO robot dependent code..
        self.hako.subscribe_pdu_channel(1, 1024, 'Ev3PduSensor')
        self.hako.register("ai_" + model_name)

    def get_actions(self):
        #TODO robot dependent code..
        binary_data = bytearray(1024)
        return { 0: binary_reader.binary_read(self.hako.offmap, 'Ev3PduActuator', binary_data) }

    def reset(self):
        self.hako.stop()
        self.hako.reset()
        self.hako.start()

    def step(self, action):
        for channel_id in action:
            self.hako.write_pdu(channel_id, action[channel_id])
        state = self.hako.execute()
        reward = 0
        done = False
        info = "none"
        return state, reward, done, info

def make(model_name):
    return HakoEnv(model_name)

