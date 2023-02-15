#!/usr/bin/python
# -*- coding: utf-8 -*-
import json
import sys

from hako_binary import binary_io
from hako_binary import offset_parser
from hako_binary import offset_map

def binary_write(offmap, binary_data, json_data, typename):
    binary_write_recursive(offmap, binary_data, json_data, 0, typename)

def binary_write_recursive(offmap, binary_data, json_data, base_off, typename):
    #lines = offmap[typename]
    lines = offmap.get(typename)
    for key in json_data:
        line = offset_parser.select_by_name(lines, key)
        off = offset_parser.member_off(line) + base_off
        type = offset_parser.member_type(line)
        if (offset_parser.is_primitive(line)):
            if (offset_parser.is_single(line)):
                bin = binary_io.typeTobin(type, json_data[key])
                binary_io.writeBinary(binary_data, off, bin)
            else:
                i = 0
                elm_size = offset_parser.member_size(line)
                array_size = offset_parser.array_size(line)
                one_elm_size = int(elm_size / array_size)
                for elm in json_data[key]:
                    bin = binary_io.typeTobin(type, elm)
                    binary_io.writeBinary(binary_data, off + (i * one_elm_size), bin)
                    i = i + 1
        else:
            if (offset_parser.is_single(line)):
                binary_write_recursive(offmap, binary_data, json_data[key], off, type)
            else:
                i = 0
                elm_size = offset_parser.member_size(line)
                array_size = offset_parser.array_size(line)
                one_elm_size = int(elm_size / array_size)
                for elm in json_data[key]:
                    binary_write_recursive(offmap, binary_data, elm, off + (i * one_elm_size), type)
                    i = i + 1
            

