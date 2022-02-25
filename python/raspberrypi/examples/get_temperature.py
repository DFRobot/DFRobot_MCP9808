# -*- coding:utf-8 -*-
'''!
  @file get_temperature.py
  @brief The example of obtaining temp normally
  @n Experimental phenomenon: display the temp on the serial port, the measured temp varies with ambient temp
  @copyright Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
  @license The MIT License (MIT)
  @author [ZhixinLiu](zhixin.liu@dfrobot.com)
  @version V1.0
  @date 2021-04-16
  @url https://github.com/DFRobot/DFRobot_MCP9808
'''
import sys
import os

sys.path.append(os.path.dirname(os.path.dirname(os.path.realpath(__file__))))
from DFRobot_MCP9808 import *

'''
  i2c address select, default to be MCP9808_ADDRESS_7, pin A2, A1 and A0 is at high level
  1 for high level, 0 for low level,   | A2 | A1 | A0 |
  MCP9808_ADDRESS_0        | 0  | 0  | 0  |    0x18
  MCP9808_ADDRESS_1        | 0  | 0  | 1  |    0x19
  MCP9808_ADDRESS_2        | 0  | 1  | 0  |    0x1A
  MCP9808_ADDRESS_3        | 0  | 1  | 1  |    0x1B
  MCP9808_ADDRESS_4        | 1  | 0  | 0  |    0x1D
  MCP9808_ADDRESS_5        | 1  | 0  | 1  |    0x1D
  MCP9808_ADDRESS_6        | 1  | 1  | 0  |    0x1E
  MCP9808_ADDRESS_7        | 1  | 1  | 1  |    0x1F
'''
I2C_BUS         = 0x01            #default use I2C1
MCP9808_ADDRESS = MCP9808_ADDRESS_7
tmp = DFRobot_MCP9808_I2C(I2C_BUS ,MCP9808_ADDRESS_7)

def setup():
  while ERROR == tmp.sensor_init():
    print("sensor init error ,please check connect or device id or manufacturer id error")
  print("sensor init success")
  
  '''
    Sensor waking up mode, sensor data can be obtained normally at this time
  '''
  if 1 == tmp.wakeup_mode():
    print("Wake up sensor successfully, can read the temperature!")
  else:
    print("Register lock")

  '''
    Set resolution of the temperature sensor, the accuracy of the acquired temp is different in different resolution
    RESOLUTION_0_5      The decimal part of the obtained temp is a multiple of 0.5     e.g. 0.5°C, 1.0°C, 1.5°C
    RESOLUTION_0_25     The decimal part of the obtained temp is a multiple of 0.25    e.g. 0.25°C, 0.50°C, 0.75°C
    RESOLUTION_0_125    The decimal part of the obtained temp is a multiple of 0.125   e.g. 0.125°C, 0.250°C, 0.375°C
    RESOLUTION_0_0625   The decimal part of the obtained temp is a multiple of 0.0625  e.g. 0.0625°C, 0.1250°C, 0.1875°C
  '''
  if 0 == tmp.set_resolution(RESOLUTION_0_25):
    print("Resolution set successfully")
  else:
    print("parameter error")

def loop():
  temperature = tmp.get_temperature()
  print("temperature = %.2f .C"%temperature)
  print("")
  time.sleep(1)
  '''
    Sensor sleep mode, lower power consumption, cannot get sensor data properly at this time.
  '''
  '''
  if 1 == tmp.sleep_mode():
    print("sleep sensor successfully, can't read the temperature!")
  else:
    print("Register lock")
  '''
if __name__ == "__main__":
  setup()
  while True:
    loop()
