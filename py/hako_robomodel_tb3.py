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
        self.hako.create_pdu_channel(0, 48, 'Twist')
        self.hako.subscribe_pdu_channel(1, 440, 'JointState')
        self.hako.subscribe_pdu_channel(2, 432, 'Imu')
        self.hako.subscribe_pdu_channel(3, 944, 'Odometry')
        self.hako.subscribe_pdu_channel(4, 320, 'TFMessage')
        self.hako.subscribe_pdu_channel(5, 1024280, 'Image')
        self.hako.subscribe_pdu_channel(6, 102664, 'CompressedImage')
        self.hako.subscribe_pdu_channel(7, 580, 'CameraInfo')
        self.hako.subscribe_pdu_channel(8, 3044, 'LaserScan')
        self.binary_data = bytearray(48)
        self.actions = { 0: binary_reader.binary_read(self.hako.offmap, 'Twist', self.binary_data) }

    def delta_usec(self):
        #20msec
        return 20000

