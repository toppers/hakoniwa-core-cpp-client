#!/usr/bin/python
# -*- coding: utf-8 -*-
import json
import sys

from binary import binary_io
from binary import offset_parser
from binary import offset_map

def binary_read(offmap, typename, binary_data):
    json_data = {}
    binary_read_recursive(offmap, binary_data, json_data, 0, typename)
    return json_data

def binary_read_recursive(offmap, binary_data, json_data, base_off, typename):
    #lines = offmap[typename]
    lines = offmap.get(typename)
    for line in lines:
        off = offset_parser.member_off(line) + base_off
        type = offset_parser.member_type(line)
        name = offset_parser.member_name(line)
        size = offset_parser.member_size(line)
        if (offset_parser.is_primitive(line)):
            if (offset_parser.is_single(line)):
                bin = binary_io.readBinary(binary_data, off, size)
                value = binary_io.binTovalue(type, bin)
                json_data[name] = value
            else:
                i = 0
                array_size = offset_parser.array_size(line)
                one_elm_size = int(size / array_size)
                array_value = []
                while i < array_size:
                    toff =  off + (i * one_elm_size)
                    tsize = one_elm_size
                    bin = binary_io.readBinary(binary_data, toff, tsize)
                    value = binary_io.binTovalue(type, bin)
                    array_value.append(value)
                    i = i + 1
                json_data[name] = array_value
        else:
            if (offset_parser.is_single(line)):
                tmp_json_data = {}
                binary_read_recursive(offmap, binary_data, tmp_json_data, off, type)
                json_data[name] = tmp_json_data
            else:
                i = 0
                array_size = offset_parser.array_size(line)
                one_elm_size = int(size / array_size)
                array_value = []
                while i < array_size:
                    tmp_json_data = {}
                    binary_read_recursive(offmap, binary_data, tmp_json_data, off + (i * one_elm_size), type)
                    array_value.append(tmp_json_data)
                    i = i + 1
                json_data[name] = array_value


