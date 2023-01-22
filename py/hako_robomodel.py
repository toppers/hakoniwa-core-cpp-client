#!/usr/bin/python
# -*- coding: utf-8 -*-
from binary import offset_map
from binary import binary_writer
from binary import binary_reader
import hako_robomodel_ev3


def create(robo_type, hako):
    if robo_type == "ev3":
        return hako_robomodel_ev3.HakoRoboModelEv3(hako)
    else:
        return None
