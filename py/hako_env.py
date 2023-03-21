#!/usr/bin/python
# -*- coding: utf-8 -*-
import json
import sys
from hako_binary import offset_map
from hako_binary import binary_writer
from hako_binary import binary_reader
import hako


class HakoEnv:
    def __init__(self, model_name, robo_type):
        offset_path = '/usr/local/lib/hakoniwa/py/hako_binary/offset'
        self.hako = hako.Hako(offset_path, robo_type)
        self.hako.register("ai_" + model_name, model_name)

    def robo(self):
        return self.hako.robo

    def reset(self):
        if self.hako.state() == hako.HakoState['RUNNING']:
            self.hako.stop()
            self.hako.reset()
        self.hako.start()

    def step(self, action):
        for channel_id in self.robo().actions:
            self.hako.write_pdu(channel_id, self.robo().actions[channel_id])
        
        self.robo().action(action)
        obserbation = self.hako.execute()
        state = self.robo().state(obserbation)
        reward, done = self.robo().rewaord(obserbation)
        info = "none"
        return state, reward, done, info

def make(model_name, robo_type):
    return HakoEnv(model_name, robo_type)

