#!/usr/bin/python
# -*- coding: utf-8 -*-
import json
import sys
from binary import offset_map
from binary import binary_writer
from binary import binary_reader

if len(sys.argv) != 5:
	print("Usage: binary_writer.py <offset_path> <typename> <binary_def_json> <outfile>")
	sys.exit()

offset_path=sys.argv[1] 
typename=sys.argv[2]
binary_def_json=sys.argv[3]
outfile=sys.argv[4]
json_open = open(binary_def_json, 'r')
base_json_data = json.load(json_open)

offmap = offset_map.create_offmap(offset_path)
binary_data = bytearray(1024)

binary_writer.binary_write(offmap, binary_data, base_json_data, typename)

value = binary_reader.binary_read(offmap, typename, binary_data)

print(json.dumps(value))


