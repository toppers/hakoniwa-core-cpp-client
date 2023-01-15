#!/usr/bin/python
# -*- coding: utf-8 -*-

import glob
import re
import os
from sys import byteorder
import struct

def binToint8(binary):
    return struct.unpack('b', binary)[0]

def binToint16(binary):
    return struct.unpack('h', binary)[0]

def binToint32(binary):
    return struct.unpack('i', binary)[0]

def binToint64(binary):
    return struct.unpack('q', binary)[0]

def binTouint8(binary):
    return struct.unpack('B', binary)[0]

def binTouint16(binary):
    return struct.unpack('H', binary)[0]

def binTouint32(binary):
    return struct.unpack('I', binary)[0]

def binTouint64(binary):
    return struct.unpack('Q', binary)[0]

def binTofloat32(binary):
    return struct.unpack('f', binary)[0]

def binTofloat64(binary):
    return struct.unpack('d', binary)[0]

def binTostring(binary):
    return binary.decode().replace('\0', '')


def int8Tobin(arg):
    return struct.pack('b', arg)

def int16Tobin(arg):
    return struct.pack('h', arg)

def int32Tobin(arg):
    return struct.pack('i', arg)

def int64Tobin(arg):
    return struct.pack('q', arg)

def uint8Tobin(arg):
    return struct.pack('B', arg)

def uint16Tobin(arg):
    return struct.pack('H', arg)

def uint32Tobin(arg):
    return struct.pack('I', arg)

def uint64Tobin(arg):
    return struct.pack('Q', arg)

def float32Tobin(arg):
    return struct.pack('f', arg)

def float64Tobin(arg):
    return struct.pack('d', arg)

def stringTobin(arg):
    return arg.encode(encoding='utf-8')

def typeTobin(type, arg):
    if (type == "int8"):
        return int8Tobin(arg)
    elif (type == "uint8"):
        return uint8Tobin(arg)
    elif (type == "int16"):
        return int16Tobin(arg)
    elif (type == "uint16"):
        return uint16Tobin(arg)
    elif (type == "int32"):
        return int32Tobin(arg)
    elif (type == "uint32"):
        return uint32Tobin(arg)
    elif (type == "int64"):
        return int64Tobin(arg)
    elif (type == "uint64"):
        return uint64Tobin(arg)
    elif (type == "float32"):
        return float32Tobin(arg)
    elif (type == "float64"):
        return float64Tobin(arg)
    elif (type == "string"):
        return stringTobin(arg)
    else:
        return None

def binTovalue(type, arg):
    if (type == "int8"):
        return binToint8(arg)
    elif (type == "uint8"):
        return binTouint8(arg)
    elif (type == "int16"):
        return binToint16(arg)
    elif (type == "uint16"):
        return binTouint16(arg)
    elif (type == "int32"):
        return binToint32(arg)
    elif (type == "uint32"):
        return binTouint32(arg)
    elif (type == "int64"):
        return binToint64(arg)
    elif (type == "uint64"):
        return binTouint64(arg)
    elif (type == "float32"):
        return binTofloat32(arg)
    elif (type == "float64"):
        return binTofloat64(arg)
    elif (type == "string"):
        return binTostring(arg)
    else:
        return None

def writeBinary(binary_data, off, bin):
    i = 0
    for data in bin:
        binary_data[off + i] = data
        i = i + 1

def readBinary(binary_data, off, size):
    return binary_data[off:off+size]

def find_filepath(path, filename):
    f_array = filename.split('/')
    if (len(f_array) > 1):
        filename = f_array[len(f_array) - 1]
    tmp = glob.glob(path + filename, recursive=True)
    if (len(tmp) == 0):
        print("ERROR: find_filepath(" + path + " ," + filename +")" )
        exit(1)
    return tmp[0]

