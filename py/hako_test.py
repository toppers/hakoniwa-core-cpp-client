#!/usr/bin/python
# -*- coding: utf-8 -*-
import json
import sys
from binary import offset_map
from binary import binary_writer
from binary import binary_reader
import hako
import time

#TODO get ai model
pass

#create hakoniwa env
env = hako.make("ev3")

actions = env.get_actions()
#do simulation
total_time = 0
done = False
while not done and total_time < 10000:
    state, reward, done, info = env.step(actions)

