# -*- coding:utf-8 -*-
'''!
  @file get_all_state.py
  @brief Get all the config status routine
  @n Experimental phenomenon: serial print the sensor config status
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
    Get resolution of the temperature sensor, the accuracy of the acquired temp is different in different resolution
    resolution
    RESOLUTION_0_5    The decimal part of the obtained temp is a multiple of 0.5     e.g. 0.5°C, 1.0°C, 1.5°C
    RESOLUTION_0_25   The decimal part of the obtained temp is a multiple of 0.25    e.g. 0.25°C, 0.50°C, 0.75°C
    RESOLUTION_0_125  The decimal part of the obtained temp is a multiple of 0.125   e.g. 0.125°C, 0.250°C, 0.375°C
    RESOLUTION_0_0625 The decimal part of the obtained temp is a multiple of 0.0625  e.g. 0.0625°C, 0.1250°C, 0.1875°C
  '''
  resolution = tmp.get_resolution()
  if resolution == RESOLUTION_0_5:
    print("resolution = 0.5 .C")
  elif resolution == RESOLUTION_0_25:
    print("resolution = 0.25 .C")
  elif resolution == RESOLUTION_0_125:
    print("resolution = 0.125 .C")
  else:
    print("resolution = 0.0625 .C")

  '''
    Get the configured power mode
    Wake-up mode: in this mode, the register can be accessed and the temp can be obtained normally.；
    Sleep mode: temp measurement stop and the register can be read or written, but bus activity will cause higher power consumption
  '''
  if 0 == tmp.get_power_mode():
    print("sleep mode")
  else:
    print("wakeup mode")

  '''
    Get alert output mode
      COMPARATOR_OUTPUT_MODE     Comparator output mode
      INTERRPUT_OUTPUT_MODE      Interrupt output mode
  '''
  state = tmp.get_alert_output_mode()
  if state == COMPARATOR_OUTPUT_MODE:
    print("Comparator output mode")
  elif state == INTERRPUT_OUTPUT_MODE:
    print("Interrupt output mode")
  else:
    print("Disable output mode")

  '''
    Get pin ALE polarity, high polarity: pin ALE is active on high, the default is low level, which turns to high when alert occurs.
    low polarity: pin ALE is active on low, the default is high level, which turns to low when alert occurs.
    POLARITY_HIGH    high polarity
    POLARITY_LOW     low polarity
  '''
  state = tmp.get_polarity_state()
  if state == POLARITY_HIGH:
    print("Pin polarity is high")
  else:
    print("Pin polarity is low")

  '''
    Get response mode, respond to upper & lower limits and critical value, or only respond to critical value
    UPPER_LOWER_CRIT_RESPONSE       enable upper/lower limits and threshold response
    ONLY_CRIT_RESPONSE              disable upper/lower limits response, only enable threshold response
  '''
  state = tmp.get_alert_response_mode()
  if state == UPPER_LOWER_CRIT_RESPONSE:
    print("Both upper/lower and crti values respond")
  else:
    print("Critical response")

  '''
    Set alert temp hysteresis range, add a range to the upper limit, lower limit and threshold, hysteresis function is only applicable to reduce temperature (from hot to cold)
    i.e, ALE level will be restored only when the temp reaches the value obtained by subtracting hysteresis temp from the upper limit
    For example: maximum temp is 30.0℃, hysteresis temp is +1.5℃, the current temp is 35℃ and ALE level has flipped,
    and then the ALE level can only be restored when the temp goes to 30-1.5=28.5℃
    HYSTERESIS_0_0      No hysteresis, respond when reaching the specified temp
    HYSTERESIS_1_5      1.5℃ lag from hot to cold
    HYSTERESIS_3_0      3.0℃ lag from hot to cold
    HYSTERESIS_6_0      6.0℃ lag from hot to cold
  '''
  state = tmp.get_alert_hysteresis()
  if state == HYSTERESIS_0_0:
    print("The temperature lag range is 0 degrees")
  elif state == HYSTERESIS_1_5:
    print("The temperature lag range is 1.5 degrees")
  elif state == HYSTERESIS_3_0:
    print("The temperature lag range is 3.0 degrees")
  else:
    print("The temperature lag range is 6.0 degrees")

  '''
    Get locking status, to determine if upper limit, lower limit, and threshold can be changed
    and unlock only by power-off reset after locking
    CRIT_LOCK      Lock threshold value, which is not allowed to be changed
    WIN_LOCK       Lock upper and lower thresholds, which are not allowed to be changed
    CRIT_WIN_LOCK  Lock threshold value, upper and lower limit, which are not allowed to be changed
    NO_LOCK        No locking of register
  '''
  state = tmp.get_lock_state()
  if state == CRIT_LOCK:
    print("The crit is locked")
  elif state == WIN_LOCK:
    print("The upper and lower limits are locked")
  elif state == CRIT_WIN_LOCK:
    print("The upper and lower limits and crit are locked")
  else:
    print("no locked")

def loop():
  time.sleep(0.1)
  exit()

if __name__ == "__main__":
  setup()
  while True:
    loop()
