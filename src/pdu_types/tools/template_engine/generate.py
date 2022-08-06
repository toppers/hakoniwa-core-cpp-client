#!/usr/bin/python
# -*- coding: utf-8 -*-
import json
import sys
import glob
import re
import string
from jinja2 import Template, Environment, FileSystemLoader

env = Environment(loader=FileSystemLoader('./', encoding='utf8'))

if len(sys.argv) != 4:
	print("ERROR: generate.py <tpl_file> <ros_json_file> <dep_lists>")
	sys.exit()

tpl_file=sys.argv[1] 
ros_json_file=sys.argv[2]
dep_lists=sys.argv[3]



tmp_str_array = ros_json_file.split(".")[1].split("/")
pkg_name = tmp_str_array[len(tmp_str_array) - 2]
msg_type_name = tmp_str_array[len(tmp_str_array) - 1]

class RosMessageContainer:
    pass

class RosMessage:
    pass

def to_conv(name):
	tmp = name.replace("int", "Int").replace("uInt", "UInt").replace("float", "Float").replace("string", "String").replace("bool", "Bool")
	return re.sub(r'\[.*\]', "Array", tmp)

def is_primitive(name):
	if (name == 'int8'):
		return True
	elif (name == 'uint8'):
		return True
	elif (name == 'int16'):
		return True
	elif (name == 'uint16'):
		return True
	elif (name == 'int32'):
		return True
	elif (name == 'uint32'):
		return True
	elif (name == 'int64'):
		return True
	elif (name == 'uint64'):
		return True
	elif (name == 'float32'):
		return True
	elif (name == 'float64'):
		return True
	elif (name == 'bool'):
		return True
	else:
		return False



def is_array(name):
	if (name.find('[') > 0):
		return True
	else:
		return False

def is_primitive_array(name):
	if (is_array(name) and is_primitive(get_array_type(name))):
		return True
	else:
		return False

def get_array_type(name):
	tmp = name.split('[')
	return tmp[0].strip()

def get_array_size(name):
	tmp = name.split('[')[1]
	if (tmp.split(']')[0] != ''):
		return tmp.split(']')[0]
	else:
		return "HAKO_PDU_MAX_ARRAY_SIZE"

def get_type(name):
	if (is_array(name)):
		return get_array_type(name)
	else:
		return name

def get_msg_type(name):
	if (name.find('/') > 0):
		tmp = name.split('/')[1]
	else:
		tmp = name
	if (tmp == 'time'):
		return 'Time'
	else:
		return tmp

container = RosMessageContainer()
container.to_conv = to_conv
container.get_type = get_type
container.get_array_type = get_array_type
container.is_array = is_array
container.is_primitive = is_primitive
container.is_primitive_array = is_primitive_array
container.get_msg_type = get_msg_type
container.pkg_name = pkg_name
container.msg_type_name = msg_type_name
container.get_array_size = get_array_size

container.includes = []
for line in open(dep_lists, 'r'):
	pkg_name = line.split("/")[0]
	msg_name = line.split("/")[1].strip()
	container.includes.append(pkg_name + "/pdu_ctype_" + msg_name + ".h")

tmp_file = open(ros_json_file)
container.json_data = json.load(tmp_file)

tpl = env.get_template(tpl_file)
out = tpl.render({'container':container})

print(out)

