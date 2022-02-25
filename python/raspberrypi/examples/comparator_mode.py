# -*- coding:utf-8 -*-
'''!
  @file comparator_mode.py
  @brief The example of comparator obtaining temp
  @n Experimental phenomenon: display the temp on the serial port, the measured temp varies with ambient temp
  @n Experimental phenomenon: print status of the current, the upper/lower and the critical temp, for example, the current temp is above the critical temp, below the lower limit, and above the upper limit
  @copyright Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
  @license The MIT License (MIT)
  @author [ZhixinLiu](zhixin.liu@dfrobot.com)
  @version V1.0
  @date 2021-04-16
  @url https://github.com/DFRobot/DFRobot_MCP9808
'''
from __future__ import print_function
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
  
  '''
    Set alert output mode, clearing interrupt is not required in comparator output mode, but required in interrupt mode
    COMPARATOR_OUTPUT_MODE        clearing interrupt is not required in comparator output mode, 
        For example: set pin ALE to be active on low, when the temp exceeds the upper limit alert temp, pin ALE level turns from high to low; when it's below the upper but above the lower limit, pin ALE level turns back to high
    INTERRPUT_OUTPUT_MODE         clearing interrupt is required in interrupt mode, when an alert occurs, the interrupt keep existing if it's not cleared, triggering interrupt mode means status changes from one to another
        For example, set lower threshold to be 20℃, upper threshold 25℃, and threshold 30℃, no interrupt occurs when temp is below 20℃; when temp exceeds 25℃, interrupt occurs and pin ALE jump, the interrupt should be cleared at the time and pin ALE is restored; 
        in the special case, when the current temp is above the critical temp of 30℃, the interrupt mode and clearing interrupt are both invalid, the interrupt mode will be restored only when the temp drops below 30℃.
    DISABLE_OUTPUT_MODE           After disabling output mode, no alert occurs and pin ALE is invalid.
  '''
  if 0 == tmp.set_alert_output_mode(COMPARATOR_OUTPUT_MODE):
    print("The comparator mode was set successfully")
  else:
    print("Register lock or parameter error")

  '''
    Set pin ALE polarity
    POLARITY_HIGH     Set pin ALE to be active on high, the default is low level, which turns to high when alert occurs.
    POLARITY_LOW      Set pin ALE to be active on low, the default is high level, which turns to high when alert occurs.
  '''
  if 0 == tmp.set_polarity(POLARITY_LOW):
    print("Pin polarity set successfully")
  else:
    print("Register lock or parameter error")

  '''
    Set alert response mode, respond to upper & lower limits and threshold; or only respond to threshold, which is inapplicable to interrupt mode
    UPPER_LOWER_CRIT_RESPONSE        enable upper/lower limits and threshold response
    ONLY_CRIT_RESPONSE               disable upper/lower limits response, only enable threshold response
  '''
  if 0 == tmp.set_alert_response_mode(UPPER_LOWER_CRIT_RESPONSE):
    print("Set response mode successfully")
  else:
    print("Register lock or parameter error")

  '''
    Set upper and lower threshold, the response occur according to the configured interrupt mode, there is response when the temp is above upper and below lower temp, up to two decimal places.
    upper The set upper limit temp, automatically processed to multiples of 0.25, range is from -40 to +125℃
    lower The set lower limit temp, automatically processed to multiples of 0.25, range is from -40 to +125℃
  '''
  if 0 == tmp.set_threshold(35.0, 30.0, 20.0):
    print("Set threshold successfully")
  else:
    print("data error or Register lock")
  
  '''
    Set alert temp hysteresis range, add a range to the upper limit, lower limit and threshold, hysteresis function is only applicable to reduce temperature (from hot to cold),
    i.e, ALE level will be restored only when the temp reaches the value obtained by subtracting hysteresis temp from the upper limit
    For example: maximum temp is 30.0℃, hysteresis temp is +1.5℃, the current temp is 35 degrees and ALE level has flipped, the temp must reach 30-1.5(28.5) degrees before restoring ALE level.
    HYSTERESIS_0_0  No hysteresis, respond when reaching the specified temp
    HYSTERESIS_1_5  1.5℃ lag from hot to cold
    HYSTERESIS_3_0  3.0℃ lag from hot to cold
    HYSTERESIS_6_0  6.0℃ lag from hot to cold
  '''
  if 0 == tmp.set_alert_hysteresis(HYSTERESIS_1_5):
    print("Set hysteresis temperature successfully")
  else:
    print("Register lock or parameter error")
    
  '''
    Set lock mode or unlock, prevent wrong operations that may change upper limit, lower limit and critical value
    unlock only by power-off reset after locking
    CRIT_LOCK       Lock threshold value, which is not allowed to be changed
    WIN_LOCK        Lock upper and lower thresholds, which are not allowed to be changed
    CRIT_WIN_LOCK   Lock threshold value, upper and lower limit, which are not allowed to be changed
    NO_LOCK         No locking of register
  '''
  if 0 == tmp.set_lock_state(NO_LOCK):
    print("locked successfully")
  else:
    print("parameter error")
  if 0 == tmp.set_alert_enable(ENABLE_ALERT):
    print ("enable success")

def loop():
  '''
    Get the current comparator status and temp data, which works only in comparator mode
    List storing the current data
    [0] The current temp
    [1] Character string of the comparator status, compare the current temp with the upper threshold, lower threshold, and critical value
  '''
  data = tmp.get_comparator_state()
  print("TA = %.2f C"%data[0] ,end = ' ')
  print("state is %s"%data[1])
  time.sleep(1)

if __name__ == "__main__":
  setup()
  while True:
    loop()
