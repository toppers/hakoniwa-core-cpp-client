#!/usr/bin/python
# -*- coding: utf-8 -*-
from hako_binary import offset_map
from hako_binary import binary_writer
from hako_binary import binary_reader
import hako_robomodel_ev3
import hako_robomodel_tb3
import hako_robomodel_any
import hako_robomodel_any_client


def create(hako, robo_type, model_filepath = None):
    if robo_type == "ev3":
        return hako_robomodel_ev3.HakoRoboModelEv3(hako)
    elif robo_type == "tb3":
        return hako_robomodel_tb3.HakoRoboModelTb3(hako)
    elif robo_type == "any":
        return hako_robomodel_any.HakoRoboModelAny(hako, model_filepath)
    elif robo_type == "any_client":
        return hako_robomodel_any_client.HakoRoboModelAnyClient(hako, model_filepath)
    else:
        raise ValueError("Invalid robot type. Expected 'ev3', 'tb3', 'any' or 'any_client'.")
