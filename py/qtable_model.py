#!/usr/bin/python
# -*- coding: utf-8 -*-

import numpy as np

class QtableModel:
    def __init__(self, num_states, num_actions):
        #q_tableを乱数で初期化
        self.q_table = np.random.uniform(0, 1, (num_states,num_actions))

    def get_action(self, next_state):
        next_action = np.argmax(self.q_table[next_state])
        return next_action

    def learn(self, state, action, reward, next_state):
        gamma=0.99
        alpha=0.3
        self.q_table[state, action] = \
            (1 - alpha) * self.q_table[state, action] + \
            alpha * (reward + gamma * max(self.q_table[next_state]))

def get_model(num_states, num_actions):
    return QtableModel(num_states, num_actions)
