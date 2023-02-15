#!/usr/bin/python
# -*- coding: utf-8 -*-
from hako_binary import offset_map
from hako_binary import binary_writer
from hako_binary import binary_reader
import hako_robomodel_ev3


def create(hako, robo_type):
    if robo_type == "ev3":
        return hako_robomodel_ev3.HakoRoboModelEv3(hako)
    else:
        return None
