#!/usr/bin/python
# -*- coding: utf-8 -*-
import json
import sys
from binary import offset_map
from binary import binary_writer
from binary import binary_reader
import hako
import time

HakoEv3Motor = {
    'LEFT': 0,
    'RIGHT': 1,
    'ARM': 2
}


HakoEv3Color = {
  'NONE': 0,
  'BLACK': 1,
  'BLUE': 2,
  'GREEN': 3,
  'YELLOW': 4,
  'RED': 5,
  'WHITE': 6,
  'BROWN': 7
}
HakoEv3ColorName = {
    0: 'None',
    1: 'BLACK',
    2: 'BLUE',
    3: 'GREEN',
    4: 'YELLOW',
    5: 'RED',
    6: 'WHITE',
    7: 'BROWN'
}

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
        self.actions[0]['motors'][HakoEv3Motor['LEFT']]['power'] = speed
        self.actions[0]['motors'][HakoEv3Motor['RIGHT']]['power'] = speed
    
    def turn(self, speed):
        if speed > 0:
            self.actions[0]['motors'][HakoEv3Motor['LEFT']]['power'] = speed
            self.actions[0]['motors'][HakoEv3Motor['RIGHT']]['power'] = 0
        else:
            self.actions[0]['motors'][HakoEv3Motor['LEFT']]['power'] = 0
            self.actions[0]['motors'][HakoEv3Motor['RIGHT']]['power'] = -speed
            
    def stop(self):
            self.actions[0]['motors'][HakoEv3Motor['LEFT']]['power'] = 0
            self.actions[0]['motors'][HakoEv3Motor['RIGHT']]['power'] = 0
    
    def arm_move(self, speed):
            self.actions[0]['motors'][HakoEv3Motor['ARM']]['power'] = speed
    
    def arm_stop(self):
            self.actions[0]['motors'][HakoEv3Motor['ARM']]['power'] = 0
    
    
    def ultrasonic_sensor(self, state):
        return state[1]['sensor_ultrasonic']
    
    def color_sensors(self, state):
        return state[1]['color_sensors']

    def touch_sensor(self, state, id):
        return state[1]['touch_sensors'][id]['value']
