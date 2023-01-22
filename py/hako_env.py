#!/usr/bin/python
# -*- coding: utf-8 -*-
import json
import sys
from binary import offset_map
from binary import binary_writer
from binary import binary_reader
import hako


class HakoEnv:
    def __init__(self, model_name, robo_type):
        offset_path = '/usr/local/lib/hakoniwa/binary/offset'
        self.hako = hako.Hako(offset_path, robo_type)
        self.hako.register("ai_" + model_name)

    def robo(self):
        return self.hako.robo

    def actions(self):
        return self.hako.robo.actions

    def reset(self):
        self.hako.stop()
        self.hako.reset()
        self.hako.start()

    def step(self):
        for channel_id in self.robo.actions:
            self.hako.write_pdu(channel_id, self.robo.actions[channel_id])
        state = self.hako.execute()
        reward = 0
        done = False
        info = "none"
        return state, reward, done, info

def make(model_name, robo_type):
    return HakoEnv(model_name, robo_type)

