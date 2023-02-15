#!/usr/bin/python
# -*- coding: utf-8 -*-
import json
import sys
from hako_binary import offset_map
from hako_binary import binary_writer
from hako_binary import binary_reader
import os

if len(sys.argv) != 4:
	print("Usage: binary_read.py <offset_path> <typename> <infile>")
	sys.exit()

offset_path=sys.argv[1] 
typename=sys.argv[2]
filepath=sys.argv[3]

offmap = offset_map.create_offmap(offset_path)
binary_data = bytearray(1024)

f = open(filepath, 'rb')
f.seek(0, os.SEEK_SET)
binary_data = f.read(1024)
f.close()

value = binary_reader.binary_read(offmap, typename, binary_data)

print(json.dumps(value))


