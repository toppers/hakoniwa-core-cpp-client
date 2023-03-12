#!/usr/bin/python
# -*- coding: utf-8 -*-
from hako_binary import offset_map
from hako_binary import binary_writer
from hako_binary import binary_reader
import hako_robomodel_ev3
import hako_robomodel_tb3


def create(hako, robo_type):
    if robo_type == "ev3":
        return hako_robomodel_ev3.HakoRoboModelEv3(hako)
    elif robo_type == "tb3":
        return hako_robomodel_tb3.HakoRoboModelTb3(hako)
    else:
        return None
