#!/usr/bin/python
# -*- coding: utf-8 -*-
import json
import sys
import base64

from hako_binary import binary_io
from hako_binary import offset_parser
from hako_binary import offset_map

def decode_base64(data):
    return base64.b64decode(data)

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
                array_size = offset_parser.array_size(line)
                encode_type = "binary"
                array_value = binary_io.readBinary(binary_data, off, size)
                json_data[name + '_encode_type'] = encode_type
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


