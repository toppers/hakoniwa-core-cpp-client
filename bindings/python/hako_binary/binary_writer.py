#!/usr/bin/python
# -*- coding: utf-8 -*-
import json
import sys

from hako_binary import binary_io
from hako_binary import offset_parser
from hako_binary import offset_map
from binary_io import PduMetaData

class DynamicAllocator:
    def __init__(self):
        self.data = bytearray()
        self.offset_map = {}

    def add(self, bytes_data, key=None):
        offset = len(self.data)
        self.data.extend(bytes_data)
        if key:
            self.offset_map[key] = offset
        return offset

    def to_array(self):
        return self.data

    def size(self):
        return len(self.data)

    def get_offset(self, key):
        return self.offset_map.get(key, None)

class BinaryWriterContainer:
    def __init__(self):
        self.heap_allocator = DynamicAllocator()
        self.meta = PduMetaData()
        self.meta.set_empty()

def binary_write(offmap, binary_data, json_data, typename):
    base_allocator = DynamicAllocator()
    bw_container = BinaryWriterContainer()
    binary_write_recursive(bw_container, offmap, base_allocator, json_data, typename)

    # メタデータの設定
    total_size = base_allocator.size() + bw_container.heap_allocator.size() + PduMetaData.size()
    bw_container.meta.total_size = total_size
    bw_container.meta.heap_off = PduMetaData.size() + base_allocator.size()

    # binary_data のサイズを total_size に調整
    if len(binary_data) < total_size:
        binary_data.extend(bytearray(total_size - len(binary_data)))
    elif len(binary_data) > total_size:
        binary_data = binary_data[:total_size]

    # メタデータをバッファにコピー
    binary_io.writeBinary(binary_data, 0, bw_container.meta.to_bytes())

    # 基本データをバッファにコピー
    binary_io.writeBinary(binary_data, bw_container.meta.base_off, base_allocator.to_array())

    # ヒープデータをバッファにコピー
    binary_io.writeBinary(binary_data, bw_container.meta.heap_off, bw_container.heap_allocator.to_array())

def binary_write_recursive(bw_container: BinaryWriterContainer, offmap, allocator, json_data, typename):
    #lines = offmap[typename]
    lines = offmap.get(typename)
    for key in json_data:
        line = offset_parser.select_by_name(lines, key)
        if line is None:
            continue
        type = offset_parser.member_type(line)
        if (offset_parser.is_primitive(line)):
            if (offset_parser.is_single(line)):
                bin = binary_io.typeTobin(type, json_data[key])
                allocator.add(bin)
            elif (offset_parser.is_array(line)):
                i = 0
                for elm in json_data[key]:
                    bin = binary_io.typeTobin(type, elm)
                    allocator.add(bin)
                    i = i + 1
            else: #varray
                i = 0
                for elm in json_data[key]:
                    bin = binary_io.typeTobin(type, elm)
                    bw_container.heap_allocator.add(bin)
                    i = i + 1
        else:
            if (offset_parser.is_single(line)):
                binary_write_recursive(bw_container, offmap, allocator, json_data[key], type)
            elif (offset_parser.is_array(line)):
                i = 0
                elm_size = offset_parser.member_size(line)
                array_size = offset_parser.array_size(line)
                for elm in json_data[key]:
                    binary_write_recursive(bw_container, offmap, allocator, elm, type)
                    i = i + 1
            else: #varray
                i = 0
                array_size = len(json_data[key])
                for elm in json_data[key]:
                    binary_write_recursive(bw_container, offmap, bw_container.heap_allocator, elm, type)
                    i = i + 1
            

