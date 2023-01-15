#!/usr/bin/python
# -*- coding: utf-8 -*-
import json
import sys

import binary_io
import offset_parser

def binary_write(offmap, binary_data, json_data, base_off, typename):
    binary_write_recursive(offmap, binary_data, json_data, 0, typename)

def binary_write_recursive(offmap, binary_data, json_data, base_off, typename):
    lines = offmap[typename]
    for v in json_data['fields']:
        line = offset_parser.select_by_name(lines, v['name'])
        off = offset_parser.member_off(line) + base_off
        type = offset_parser.member_type(line)
        if (offset_parser.is_primitive(line)):
            if (offset_parser.is_single(line)):
                bin = binary_io.typeTobin(type, v['value'])
                binary_io.writeBinary(binary_data, off, bin)
            else:
                i = 0
                elm_size = offset_parser.member_size(line)
                array_size = offset_parser.array_size(line)
                one_elm_size = int(elm_size / array_size)
                for elm in v['value']:
                    bin = binary_io.typeTobin(type, elm)
                    binary_io.writeBinary(binary_data, off + (i * one_elm_size), bin)
                    i = i + 1
        else:
            if (offset_parser.is_single(line)):
                binary_write_recursive(offmap, binary_data, v['value'], off, type)
            else:
                i = 0
                elm_size = offset_parser.member_size(line)
                array_size = offset_parser.array_size(line)
                one_elm_size = int(elm_size / array_size)
                for elm in v['value']:
                    binary_write_recursive(offmap, binary_data, elm, off + (i * one_elm_size), type)
                    i = i + 1
            

