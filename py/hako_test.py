#!/usr/bin/python
# -*- coding: utf-8 -*-
import json
import sys
from hako_binary import offset_map
from hako_binary import binary_writer
from hako_binary import binary_reader
import qtable_model
import hako_env
import time
import signal
import hako_robomodel_ev3 as ev3

def handler(signum, frame):
  print(f'SIGNAL(signum={signum})')
  sys.exit(0)

# signal.SIGALRMのシグナルハンドラを登録
signal.signal(signal.SIGINT, handler)

#create hakoniwa env
env = hako_env.make("EV3RoboModel", "ev3")

#get ai model
model = qtable_model.get_model(env.robo().num_states(), env.robo().num_actions())

#do simulation

robo = env.robo()

for episode in range(100):
  total_time = 0
  done = False
  env.reset()
  state = 0
  #100secs
  while not done and total_time < 10000:
      action = model.get_action(state)
      next_state, reward, done, _ = env.step(action)
      
      print("episode=" + str(episode))
      print("state=" + str(state))
      print("reward=" + str(reward))
      print("action=" + str(action))
      print("done=" + str(done))
      print("total_time=" + str(total_time))
      
      model.learn(state, action, reward, next_state)
      
      state = next_state    
      total_time = total_time + 1

print("END")
env.reset()
sys.exit(0)

