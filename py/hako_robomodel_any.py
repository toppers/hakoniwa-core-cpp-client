#!/usr/bin/python
# -*- coding: utf-8 -*-
import json
import sys
from hako_binary import offset_map
from hako_binary import binary_writer
from hako_binary import binary_reader
import hako
import time

class ObjectLike(dict):
    __getattr__ = dict.get

class HakoRoboModelAny:
    def __init__(self, hako, model_filepath):
        self.hako = hako
        file = open(model_filepath)
        self.model = ObjectLike(json.load(file))
        self.actions = {}
        self.channel_map = {}

        for robo in self.model.robots:
            robo = ObjectLike(robo)
            print("LOADED: " + robo.name)
            self.create_pdu_lchannel(robo.rpc_pdu_readers)
            self.create_pdu_lchannel(robo.shm_pdu_readers)
            self.subscribe_pdu_lchannel(robo.rpc_pdu_writers)
            self.subscribe_pdu_lchannel(robo.shm_pdu_writers)

    def get_state(self, name, obserbation):
        return obserbation[self.channel_map[name]]
    def get_action(self, name):
        return self.actions[self.channel_map[name]]
    
    def set_state(self, state_model):
        self.state_model = state_model
    def state(self, obserbation):
        return self.state_model.state(obserbation)

    def set_reward(self, reward_model):
        self.reward_model = reward_model
    def reward(self, obserbation):
        self.reward_model.reward(obserbation)

    def create_pdu_lchannel(self, readers):
        for reader in readers:
            reader = ObjectLike(reader)
            type = reader.type.split('/')[1]
            #print("type=" + type)
            #print("channel_id=" + str(reader.channel_id))
            #print("pdu_size=" + str(reader.pdu_size))
            self.channel_map[reader.org_name] = reader.channel_id
            self.hako.create_pdu_lchannel(reader.channel_id, reader.pdu_size, type)
            binary_data = bytearray(reader.pdu_size)
            self.actions[reader.channel_id] = binary_reader.binary_read(self.hako.offmap, type, binary_data)

    def subscribe_pdu_lchannel(self, writers):
        for writer in writers:
            writer = ObjectLike(writer)
            typename = writer.type.split('/')[1]
            self.channel_map[writer.org_name] = writer.channel_id
            print("subscribe:channel_id=" + str(writer.channel_id))
            print("subscribe:typename=" + typename)
            print("subscribe:pdu_size=" + str(writer.pdu_size))
            self.hako.subscribe_pdu_lchannel(writer.channel_id, writer.pdu_size, typename)

