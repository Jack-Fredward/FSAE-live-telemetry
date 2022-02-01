
import serial
import numpy as np
import matplotlib as plt
# import codecs


ser = serial.Serial('/dev/ttyACM0',115200)
ser.close()
ser.open()
while True:

    data = ser.readline()
    print(data.decode('utf-8'))

