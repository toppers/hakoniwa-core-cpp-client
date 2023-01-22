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
        self.offmap = offset_map.create_offmap(offset_path)
        hakoc.asset_init()
        hakoc.asset_register("ai_" + model_name)
        

    def execute(self, action):
        #check event
        #notify simtime
        #pdu create check
        #simulation mode check
        #time check
        #read pdu
        #write pdu
        state = "none"
        return True, state

    def reset(self):
        hakoc.stop()
        #check event
        hakoc.reset()
        #check event
        hakoc.start()
        #check event

    def step(self, action):
        is_step_foward = False
        while is_step_foward == False:
            is_step_foward, state = self.execute(action)
            if is_step_foward == False:
                time.sleep(0.01)

        reward = "bbb"
        done = True
        info = "ccc"
        return state, reward, done, info

def make(model_name):
    return HakoEnv(model_name)

