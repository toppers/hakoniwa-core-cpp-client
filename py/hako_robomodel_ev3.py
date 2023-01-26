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
    
    
    def ultrasonic_sensor(self, obserbation):
        return obserbation[1]['sensor_ultrasonic']
    
    def color_sensors(self, obserbation):
        return obserbation[1]['color_sensors']

    def rewaord(self, obserbation):
        value = self.ultrasonic_sensor(obserbation)
        if value >= 110 and value <= 145:
            return 0.1, False
        elif value < 110:
            return -5, False
        elif value > 145 and value <= 170:
            return -5, False
        else:
            return -10, True

    def num_actions(self):
        return 6
    
    def action(self, action_no):
        if action_no == 0:
            self.foward(50)
        elif action_no == 1:
            self.foward(20)
        elif action_no == 2:
            self.turn(20)
        elif action_no == 3:
            self.turn(50)
        elif action_no == 4:
            self.turn(-20)
        else:
            self.turn(-50)

    def num_states(self):
        return 4
    
    # 0 or 1 or 2 or 3
    def state(self, obserbation):
        o = obserbation[1]['color_sensors'][0]['reflect']
        min = 40.0
        size = 60.0
        #0 -1
        ret_o = (o - min) / size
        #print("ret_o=" + str(ret_o))
        return (int)((ret_o * 30.0) / 10.0)