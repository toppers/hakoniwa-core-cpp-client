#!/usr/bin/python
# -*- coding: utf-8 -*-
import json
import sys
from hako_binary import offset_map
from hako_binary import binary_writer
from hako_binary import binary_reader
import hako
import time

class HakoRoboModelTb3:
    def __init__(self, hako):
        self.hako = hako
        self.hako.create_pdu_lchannel(0, 48, 'Twist')
        self.hako.subscribe_pdu_lchannel(1, 440, 'JointState')
        self.hako.subscribe_pdu_lchannel(2, 432, 'Imu')
        self.hako.subscribe_pdu_lchannel(3, 944, 'Odometry')
        self.hako.subscribe_pdu_lchannel(4, 320, 'TFMessage')
        self.hako.subscribe_pdu_lchannel(5, 1024280, 'Image')
        self.hako.subscribe_pdu_lchannel(6, 102664, 'CompressedImage')
        self.hako.subscribe_pdu_lchannel(7, 580, 'CameraInfo')
        self.hako.subscribe_pdu_lchannel(8, 3044, 'LaserScan')
        self.binary_data = bytearray(48)
        self.actions = { 0: binary_reader.binary_read(self.hako.offmap, 'Twist', self.binary_data) }

    def delta_usec(self):
        #20msec
        return 20000

    def camera_data(self, sensors):
        return (sensors[6]['data'])

    def laser_scan(self, sensors):
        return (sensors[8]['ranges'])

    def get_forward_distance(self, scan_datas):
        min1 = min(scan_datas[0:15])
        min2 = min(scan_datas[345:359])
        return min(min1, min2)
            
    def get_right_distance(self, scan_datas):
        return min(scan_datas[60:120])
    
    def foward(self, speed):
        self.actions[0]['linear']['x'] = speed
    
    def turn(self, speed):
        self.actions[0]['angular']['z'] = speed
            
    def stop(self):
        self.actions[0]['linear']['x'] = 0.0
        self.actions[0]['angular']['z'] = 0.0

    def step(self):
        for channel_id in self.actions:
            self.hako.write_pdu(channel_id, self.actions[channel_id])
