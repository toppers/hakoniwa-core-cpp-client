#!/usr/bin/python
# -*- coding: utf-8 -*-
import json
import sys
from binary import offset_map
from binary import binary_writer
from binary import binary_reader
import hako_env
import time
import signal
import hako_robomodel_ev3 as ev3

def handler(signum, frame):
  print(f'SIGNAL(signum={signum})')
  sys.exit(0)

# signal.SIGALRMのシグナルハンドラを登録
signal.signal(signal.SIGINT, handler)

#TODO get ai model
pass

#create hakoniwa env
env = hako_env.make("base_practice_1", "ev3")

#do simulation
total_time = 0
done = False
env.reset()

robo = env.robo()
#10secs
while not done and total_time < 10000:
    robo.foward(10)
    state, reward, done, info = env.step()
    
    distance = robo.ultrasonic_sensor(state)
    print("distance=" + str(distance))
    color_sensors = robo.color_sensors(state)
    print("color[0]*="+str(color_sensors[0]))
    print("color[0].color=" + ev3.HakoEv3ColorName[color_sensors[0]['color']])
    #print("color[1]*="+str(color_sensors[1]))
    #print("color[1].color=" + ev3.HakoEv3ColorName[color_sensors[1]['color']])
    
    total_time = total_time + 1

print("END")
env.reset()
sys.exit(0)

