#!/usr/bin/python
# -*- coding: utf-8 -*-
import json
import sys
from binary import offset_map
from binary import binary_writer
from binary import binary_reader
import hako
import time

class HakoEv3Motor:
    LEFT = 0
    RIGHT = 1
    ARM = 2


class HakoEv3Color:
  NONE = 0
  BLACK = 1
  BLUE = 2
  GREEN = 3
  YELLOW = 4
  RED = 5
  WHITE = 6
  BROWN = 7

class HakoRoboModelEv3:
    def __init__(self, hako):
        self.hako = hako
        self.hako.create_pdu_channel(0, 1024, 'Ev3PduActuator')
        self.hako.subscribe_pdu_channel(1, 1024, 'Ev3PduSensor')
        self.binary_data = bytearray(1024)
        self.actions = { 0: binary_reader.binary_read(self.hako.offmap, 'Ev3PduActuator', self.binary_data) }

    def delta_usec(self):
        #10msec
        return 10000

    def foward(self, speed):
        self.actions[0]['motors'][(int)(HakoEv3Motor.LEFT)]['power'] = speed
        self.actions[0]['motors'][(int)(HakoEv3Motor.RIGHT)]['power'] = speed
    
    def turn(self, speed):
        if speed > 0:
            self.actions[0]['motors'][(int)(HakoEv3Motor.LEFT)]['power'] = speed
            self.actions[0]['motors'][(int)(HakoEv3Motor.RIGHT)]['power'] = 0
        else:
            self.actions[0]['motors'][(int)(HakoEv3Motor.LEFT)]['power'] = 0
            self.actions[0]['motors'][(int)(HakoEv3Motor.RIGHT)]['power'] = -speed
            
    def stop(self):
        self.actions[0]['motors'][(int)(HakoEv3Motor.LEFT)]['power'] = 0
        self.actions[0]['motors'][(int)(HakoEv3Motor.RIGHT)]['power'] = 0
    
    def arm_move(self, speed):
        self.actions[0]['motors'][(int)(HakoEv3Motor.ARM)]['power'] = speed
    
    def arm_stop(self):
        self.actions[0]['motors'][(int)(HakoEv3Motor.ARM)]['power'] = 0
