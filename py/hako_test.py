#!/usr/bin/python
# -*- coding: utf-8 -*-
import json
import sys
from binary import offset_map
from binary import binary_writer
from binary import binary_reader
import hako_env
import time

#TODO get ai model
pass

#create hakoniwa env
env = hako_env.make("base_practice_1", "ev3")

#do simulation
total_time = 0
done = False
env.reset()
print("reset done")
robo = env.robo()
while not done and total_time < 10000:
    robo.foward(10)
    state, reward, done, info = env.step()

