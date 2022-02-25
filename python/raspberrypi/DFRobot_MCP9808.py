# -*- coding: utf-8 -*
'''!
  @file DFRobot_MCP9808.py
  @brief Define the basic struct of DFRobot_MCP9808 class, the implementation of basic method
  @copyright Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
  @license The MIT License (MIT)
  @author Alexander(ouki.wang@dfrobot.com)
  @version V1.0
  @date 2021-04-16
  @url https://github.com/DFRobot/DFRobot_MCP9808
'''
import serial
import time
import smbus
import spidev
import os
import RPi.GPIO as GPIO

#GPIO.setwarnings(False)
ERROR                          = -1
NONE                           = (0x00)

MCP9808_ADDRESS_0       = 0x18
MCP9808_ADDRESS_1       = 0x19
MCP9808_ADDRESS_2       = 0x1a
MCP9808_ADDRESS_3       = 0x1b
MCP9808_ADDRESS_4       = 0x1c
MCP9808_ADDRESS_5       = 0x1d
MCP9808_ADDRESS_6       = 0x1e
MCP9808_ADDRESS_7       = 0x1f

FRU_REGISTER                   = (0x00 & 0x0F)
CONFIG_REGISTER                = (0x01 & 0x0F)
T_UPPER_REGISTER               = (0x02 & 0x0F)
T_LOWER_REGISTER               = (0x03 & 0x0F)
T_CRIT_REGISTER                = (0x04 & 0x0F)
TEMPERATURE_REGISTER           = (0x05 & 0x0F)
MANUFACTURER_REGISTER          = (0x06 & 0x0F)
DEVICE_REGISTER                = (0x07 & 0x0F)
RESOLUTION_REGISTER            = (0x08 & 0x0F)

MANUFACTURER_ID                = (0x54)
DEVICE_ID                      = (0x04)

RESOLUTION_0_5                 = (0x00 & 0x03)
RESOLUTION_0_25                = (0x01 & 0x03)
RESOLUTION_0_125               = (0x02 & 0x03)
## default resolutivn
RESOLUTION_0_0625              = (0x03 & 0x03)

## default comparator mode
COMPARATOR_OUTPUT_MODE         = 0x00
INTERRPUT_OUTPUT_MODE          = 0x01
DISABLE_OUTPUT_MODE            = 0x02

CRIT_LOCK                      = 0x80
WIN_LOCK                       = 0x40
CRIT_WIN_LOCK                  = 0xC0
## default no lock
NO_LOCK                        = 0x00
## dafault power up
POWER_UP_MODE                  = 0x00
LOW_POWER_MODE                 = 0x01

## default hysteresis
HYSTERESIS_0_0                 = 0x00
HYSTERESIS_1_5                 = 0x02
HYSTERESIS_3_0                 = 0x04
HYSTERESIS_6_0                 = 0x06

POLARITY_HIGH                  = 0x02
## default active low
POLARITY_LOW                   = 0x00

ENABLE_ALERT                   = 0x08
## default disable
DISABLE_ALERT                  = 0x00

ONLY_CRIT_RESPONSE             = 0x40
## dafault output mode
UPPER_LOWER_CRIT_RESPONSE      = 0x00

class DFRobot_MCP9808(object):
  def __init__(self ,bus):
    if bus != 0:
      self.i2cbus = smbus.SMBus(bus)

  def sensor_init(self):
    '''!
      @brief Initialize sensor, comtrast sensor chip id with manufacturer id
      @return  int type
      @retval  0 is init success
      @retval -1 chip id or manufacturer id error, check please
    '''
    device_id = self.get_device_id()
    manufaturer_id = self.get_manufacturer_id()
    if device_id == DEVICE_ID and manufaturer_id == MANUFACTURER_ID:
      return 0
    else:
      return -1

  def sleep_mode(self):
    '''!
      @brief Sensor sleep mode, lower power consumption, cannot read temp data at this time
      @return int type 
      @retval 1, set successfully
      @retval 0, register lock is not allowed to be modified
    '''
    if(self.set_power_mode(LOW_POWER_MODE) == -1):
      return 0
    else:
      return 1

  
  def wakeup_mode(self):
    '''!
      @brief Sensor wake-up mode, temp data can be read at this time
      @return int type
      @retval 1, set successfully
      @retval 0, register lock is not allowed to be modified
    '''
    if(self.set_power_mode(POWER_UP_MODE) == -1):
      return 0
    else:
      return 1

  def set_power_mode(self ,mode):
    '''!
      @brief Set power mode
      @n Wake-up mode: in this mode, the register can be accessed and the temp can be obtained normally.；
      @n Sleep mode: temp measurement stop and the register can be read or written, but bus activity will cause higher power consumption
      @param mode
      @n     POWER_UP_MODE         wake-up mode
      @n     LOW_POWER_MODE        sleep mode
      @return int type
      @retval 0 is set success
      @retval -1 The setting failed, the register is locked, please unlock it first
    '''
    if self.get_lock_state() != 0:
      return -1
    rslt = self.read_reg(CONFIG_REGISTER ,2)
    rslt[0] &= 0x06
    rslt[0] |= (mode&0x01);
    self.write_reg(CONFIG_REGISTER ,rslt)
    return 0

  
  def get_power_mode(self):
    '''!
      @brief Get the configured power mode
      @n Wake-up mode, in this mode, the register can be accessed and the temp can be obtained normally；
      @n Sleep mode: temp measurement stop and the register can be read or written, but bus activity will cause higher power consumption
      @return int type
      @retval 0 is sleep mode
      @retval 1 is wakeup mode
    '''
    rslt = self.read_reg(CONFIG_REGISTER ,2) 
    if (rslt[0]&0x01) == LOW_POWER_MODE:
      return 0
    else:
      return 1

  def get_device_id(self):
    '''!
      @brief get device id
      @return device id (DEVICE_ID:0x04)
    '''
    rslt = self.read_reg(DEVICE_REGISTER ,2)
    return rslt[0]

  
  def get_manufacturer_id(self):
    '''!
      @brief get manufacturer id
      @return manufacturer id (MANUFACTURER_ID:0x54)
    '''
    rslt = self.read_reg(MANUFACTURER_REGISTER ,2)
    return rslt[1]

  def set_resolution(self ,resolution):
    '''!
      @brief Set resolution of the temperature sensor, the accuracy of the acquired temp is different in different resolution
      @param resolution
      @n     RESOLUTION_0_5     The decimal part of the obtained temp is a multiple of 0.5     e.g. 0.5°C, 1.0°C, 1.5°C
      @n     RESOLUTION_0_25    The decimal part of the obtained temp is a multiple of 0.25    e.g. 0.25°C, 0.50°C, 0.75°C
      @n     RESOLUTION_0_125   The decimal part of the obtained temp is a multiple of 0.125   e.g. 0.125°C, 0.250°C, 0.375°C
      @n     RESOLUTION_0_0625  The decimal part of the obtained temp is a multiple of 0.0625  e.g. 0.0625°C, 0.1250°C, 0.1875°C
      @return state
      @retval 0 is set successfully
      @retval -1 The set resolution error, check the resolution parameter.
    '''
    txbuf = [0]
    if resolution == RESOLUTION_0_5 or resolution == RESOLUTION_0_25 or resolution == RESOLUTION_0_125 or resolution == RESOLUTION_0_0625:
      txbuf[0] = resolution
      self.write_reg(RESOLUTION_REGISTER ,txbuf)
      return 0
    else:
      return -1

  def get_resolution(self):
    '''!
      @brief Get resolution of the temperature sensor, the accuracy of the acquired temp is different in different resolution
      @return resolution 
      @retval RESOLUTION_0_5      The decimal part of the obtained temp is a multiple of 0.5     e.g. 0.5°C, 1.0°C, 1.5°C
      @retval RESOLUTION_0_25     The decimal part of the obtained temp is a multiple of 0.25    e.g. 0.25°C, 0.50°C, 0.75°C
      @retval RESOLUTION_0_125    The decimal part of the obtained temp is a multiple of 0.125   e.g. 0.125°C, 0.250°C, 0.375°C
      @retval RESOLUTION_0_0625   The decimal part of the obtained temp is a multiple of 0.0625  e.g. 0.0625°C, 0.1250°C, 0.1875°C
    '''
    rslt = self.read_reg(RESOLUTION_REGISTER ,1)
    return rslt[0]&0x03

  def get_temperature(self):
    '''!
      @brief Get the current ambient temp, note that different temp accuracy can be obtained by setting different resolution, unit: ℃
      @return The temp value is a floating point (unit is ℃)
    '''
    rslt = self.read_reg(TEMPERATURE_REGISTER ,2)
    rslt[0] &= 0x1F
    if rslt[0]&0x10 == 0x10:
      rslt[0] &= 0x0F
      return 256.0-(float(rslt[0]*16.0) + float(rslt[1]/16.0))
    else:
      return float(rslt[0]*16.0) + float(rslt[1]/16.0)

  def get_comparator_state(self):
    '''!
      @brief Get the current comparator status and temp data, which works only in comparator mode
      @return List storing the current data
      @n      [0] The current temp
      @n      [1] Character string of the comparator status, compare the current temp with the upper threshold, lower threshold, and critical value
      @n      [2] Value of the comparator status, explained as follows
      @n      TA for current temp, TCRIT for critical temp, TUPPER for maximum temp, TLOWER for minimum temp
      @n      1 for TA ≥ TCRIT, TA > TUPPER, TA < TLOWER
      @n      0 for TA < TCRIT, TA ≤ TUPPER, TA ≥ TLOWER
      @n      ------------------------------------
      @n      | bit7 ~ bit3 | bit2 | bit1 | bit0 |
      @n      ------------------------------------
      @n      |  reserved   |  0   |  0   |  0   |
      @n      ------------------------------------
    '''
    str1 = ""
    data = [0]*10 
    rslt = self.read_reg(TEMPERATURE_REGISTER ,2) 
    state = (rslt[0]&0xE0)>>5
    data[0] = self.get_temperature()
    if (state>>2)&0x01:
      str1 += " TA >= TCRIT ,"
    else:
      str1 += " TA < TCRIT ,"
    if (state>>1)&0x01:
      str1 += " TA > TUPPER ,"
    else:
      str1 += " TA <= TUPPER ,"
    if (state>>0)&0x01:
      str1 += " TA < TLOWER"
    else:
      str1 += " TA >= TLOWER"
    data[1] = str1
    data[2] = state
    return data
  
  def set_lock_state(self ,lock):
    '''!
      @brief Set lock mode or unlock, prevent wrong operations that may change upper limit, lower limit and critical value, 
      @n     and unlock only by power-off reset after locking
      @param lock
      @n     CRIT_LOCK       Lock threshold value, which is not allowed to be changed
      @n     WIN_LOCK        Lock upper and lower thresholds, which are not allowed to be changed
      @n     CRIT_WIN_LOCK   Lock threshold value, upper and lower limit, which are not allowed to be changed
      @n     NO_LOCK         No locking of register
      @return state
      @retval 0x00 is set successfully
      @retval 0xFE The set mode error
    '''
    rslt = self.read_reg(CONFIG_REGISTER ,2)
    if lock == CRIT_LOCK or lock == WIN_LOCK or lock == CRIT_WIN_LOCK or lock == NO_LOCK:
      rslt[1] &= 0x3F
      rslt[1] |= lock
      self.write_reg(CONFIG_REGISTER ,rslt)
      return 0
    else:
      return 0xfe

  def get_lock_state(self):
    '''!
      @brief Get locking status, to determine if upper limit, lower limit, and threshold can be changed
      @return state
      @retval CRIT_LOCK      Lock threshold value, which is not allowed to be changed
      @retval WIN_LOCK       Lock upper and lower thresholds, which are not allowed to be changed
      @retval CRIT_WIN_LOCK  Lock threshold and the window at the same time, upper limit, lower limit, and threshold are all not allowed to be changed
      @retval NO_LOCK        No locking, threshold value, upper and lower limit are all can be changed
    '''
    rslt = self.read_reg(CONFIG_REGISTER ,2) 
    return (rslt[1]&0xC0)

  
  def set_alert_hysteresis(self ,mode):
    '''!
      @brief Set alert temp hysteresis range, add a range to the upper limit, lower limit and threshold, hysteresis function is only applicable to reduce temperature (from hot to cold)
      @n     i.e, ALE level will be restored only when the temp reaches the value obtained by subtracting hysteresis temp from the upper limit/lower limit/threshold.
      @n     For example: maximum temp is 30.0℃, hysteresis temp is +1.5℃, the current temp is 35℃ and ALE level has flipped,
      @n     and then the ALE level can only be restored when the temp goes to 30-1.5=28.5℃
      @param mode
      @n     HYSTERESIS_0_0     No hysteresis, respond when reaching the specified temp
      @n     HYSTERESIS_1_5     1.5℃ lag from hot to cold
      @n     HYSTERESIS_3_0     3.0℃ lag from hot to cold
      @n     HYSTERESIS_6_0     6.0℃ lag from hot to cold
      @return state
      @retval 0x00 is set successfully
      @retval -1 The current register is locked and not allowed to be changed
      @retval 0xFE The set resolution error, check the range.
    '''
    if self.get_lock_state() != 0:
      return -1
    rslt = self.read_reg(CONFIG_REGISTER ,2)
    if mode == HYSTERESIS_0_0 or mode == HYSTERESIS_1_5  or mode == HYSTERESIS_3_0 or mode == HYSTERESIS_6_0:
      rslt[0] &= 0x01
      rslt[0] |= mode
      self.write_reg(CONFIG_REGISTER ,rslt)
      return 0
    else:
      return 0xFE


  def get_alert_hysteresis(self):
    '''!
      @brief Get hysteresis temp
      @return hysteresis
      @retval HYSTERESIS_0_0    No hysteresis, respond when reaching the specified temp
      @retval HYSTERESIS_1_5    1.5℃ lag from hot to cold
      @retval HYSTERESIS_3_0    3.0℃ lag from hot to cold
      @retval HYSTERESIS_6_0    6.0℃ lag from hot to cold
    '''
    rslt = self.read_reg(CONFIG_REGISTER ,2) 
    return (rslt[0]&0x06)

  
  def set_polarity(self ,polarity):
    '''!
      @brief Set pin ALE polarity, high polarity: pin ALE is active on high, the default is low level, which turns to high when alert occurs.
      @n     low polarity: pin ALE is active on low, the default is high level, which turns to low when alert occurs.
      @param polarity
      @n     POLARITY_HIGH   pin ALE is active on high
      @n     POLARITY_LOW    pin ALE is active on low
      @return state
      @retval 0x00 is set successfully
      @retval -1   The current register is locked and not allowed to be changed
      @retval 0xFE The set polarity error, check the polarity.
    '''
    if self.get_lock_state() != 0:
      return -1
    rslt = self.read_reg(CONFIG_REGISTER ,2)
    if polarity == POLARITY_HIGH or polarity == POLARITY_LOW:
      rslt[1] &= 0xFD
      rslt[1] |= polarity
      self.write_reg(CONFIG_REGISTER ,rslt)
      return 0
    else:
      return 0xFE

  def get_polarity_state(self):
    '''!
      @brief Get pin ALE polarity, high polarity: pin ALE is active on high, the default is low level, which turns to high when alert occurs.
      @n     low polarity: pin ALE is active on low, the default is high level, which turns to low when alert occurs.
      @return polarity
      @retval POLARITY_HIGH   pin ALE is active on high
      @retval POLARITY_LOW    pin ALE is active on low
    '''
    rslt = self.read_reg(CONFIG_REGISTER ,2) 
    return (rslt[1]&0x02)

  
  def set_alert_output_mode(self ,mode):
    '''!
      @brief Set alert output mode, clearing interrupt is not required in comparator output mode, but required in interrupt mode
      @param mode
      @n     COMPARATOR_OUTPUT_MODE           clearing interrupt is not required in comparator output mode, 
      @n      For example: set pin ALE to be active on low, when the temp exceeds the upper limit alert temp, pin ALE level turns from high to low; when it's below the upper but above the lower limit, pin ALE level turns back to high
      @n     INTERRPUT_OUTPUT_MODE            clearing interrupt is required in interrupt mode, when an alert occurs, the interrupt keep existing if it's not cleared, triggering interrupt mode means status changes from one to another
      @n      For example, set lower threshold to be 20℃, upper threshold 25℃, and threshold 30℃, no interrupt occurs when temp is below 20℃; when temp exceeds 25℃, interrupt occurs and pin ALE jump, the interrupt should be cleared at the time and pin ALE is restored; 
      @n      in the special case, when the current temp is above the critical temp of 30℃, the interrupt mode and clearing interrupt are both invalid, the interrupt mode will be restored only when the temp drops below 30℃.
      @n     DISABLE_OUTPUT_MODE              After disabling output mode, no alert occurs and pin ALE is invalid.
      @return state
      @retval 0x00 is set successfully
      @retval -1 The current register is locked and not allowed to be changed
      @retval 0xFE The set alert output mode error, check the mode.
    '''
    if self.get_lock_state() != 0:
      return -1
    rslt = self.read_reg(CONFIG_REGISTER ,2)
    if mode == COMPARATOR_OUTPUT_MODE or mode == INTERRPUT_OUTPUT_MODE:
      rslt[1] &= 0xFE
      rslt[1] |= mode
      self.write_reg(CONFIG_REGISTER ,rslt)                 # enable alert mode
      self.set_alert_enable(ENABLE_ALERT)
      return 0x00
    elif mode == DISABLE_OUTPUT_MODE:
      self.set_alert_enable(DISABLE_ALERT)                  # disable alert mode
      return 0x00
    else:
      return 0xFE

  
  def get_alert_output_mode(self):
    '''!
      @brief Get alert output mode
      @return mode
      @retval COMPARATOR_OUTPUT_MODE     Comparator output mode
      @retval INTERRPUT_OUTPUT_MODE      Interrupt output mode
      @retval DISABLE_OUTPUT_MODE        Disable output mode
    '''
    if self.get_alert_enable_state() == DISABLE_ALERT:
      return DISABLE_OUTPUT_MODE
    else:
      rslt = self.read_reg(CONFIG_REGISTER ,2) 
      return (rslt[1]&0x01)

  
  def set_alert_enable(self ,mode):
    '''!
      @brief Enable or disable alert mode, after enabling it, pin ALE will jump when satisfying the alert condition; if disabling it, pin ALE has no response
      @param mode
      @n     ENABLE_ALERT      after enabling alert mode, pin ALE will jump when satisfying the alert condition
      @n     DISABLE_ALERT     when disabling alert mode, pin ALE has no response
      @return state
      @retval 0x00 is set successfully
      @retval -1   The current register is locked and not allowed to be changed
      @retval 0xFE The set mode error, check the mode.
    '''
    if self.get_lock_state() != 0:
      return -1
    rslt = self.read_reg(CONFIG_REGISTER ,2)
    if mode == ENABLE_ALERT or mode == DISABLE_ALERT:
      rslt[1] &= 0xF7
      rslt[1] |= mode
      self.write_reg(CONFIG_REGISTER ,rslt)
      return 0
    else:
      return 0xFE

  
  def get_alert_enable_state(self):
    '''!
      @brief Get the alert mode status, enable alert mode or disable it
      @return mode
      @retval ENABLE_ALERT  enable alert mode
      @retval DISABLE_ALERT disable alert mode
    '''
    rslt = self.read_reg(CONFIG_REGISTER ,2) 
    return (rslt[1]&0x08)

  
  def set_alert_response_mode(self ,mode):
    '''!
      @brief Set alert response mode, respond to upper & lower limits and threshold; or only respond to threshold, which is inapplicable to interrupt mode
      @param mode
      @n     UPPER_LOWER_CRIT_RESPONSE       enable upper/lower limits and threshold response
      @n     ONLY_CRIT_RESPONSE              disable upper/lower limits response, only enable threshold response
      @return state
      @retval 0x00 is set successfully
      @retval -1   The current register is locked and not allowed to be changed
      @retval 0xFE The set response mode error, check the mode.
    '''
    if self.get_lock_state() != 0:
      return -1
    rslt = self.read_reg(CONFIG_REGISTER ,2)
    if mode == UPPER_LOWER_CRIT_RESPONSE or mode == ONLY_CRIT_RESPONSE:
      rslt[1] &= 0xFB
      rslt[1] |= mode
      self.write_reg(CONFIG_REGISTER ,rslt)
      return 0
    else:
      return 0xFE

  
  def get_alert_response_mode(self):
    '''!
      @brief Get response mode
      @return mode
      @retval UPPER_LOWER_CRIT_RESPONSE    enable upper/lower limits and threshold response
      @retval ONLY_CRIT_RESPONSE           disable upper/lower limits response, only enable threshold response
    '''
    rslt = self.read_reg(CONFIG_REGISTER ,2) 
    return (rslt[1]&0x04)

  
  def set_threshold(self, crit, upper, lower):
    '''!
      @brief Set threshold value, the upper and lower thresholds, and respond according to the configured interrupt mode 
      @n     Responding interrupt when the temp is above the upper limit, below the lower limit and above the threshold
      @n     the critical temp must be above the upper limit
      @n     The upper limit temp must be 2℃ higher than the lower limit temp
      @param crit  Temp threshold, up to two decimal places, automatically processed to multiples of 0.25, range is from -40 to +125℃
      @param upper Upper limit temp, up to two decimal places, automatically processed to multiples of 0.25, range is from -40 to +125℃
      @param lower Lower limit temp, up to two decimal places, automatically processed to multiples of 0.25, range is from -40 to +125℃
      @return state
      @retval 0  is set successfully
      @retval -3 The critical temp is below the upper limit
      @retval -2 Upper limit temp is below lower limit, or (upper limit temp - lower limit temp < 2)
      @retval -1 The register is locked and can't be changed.
    '''
    rslt = [0]*2
    if crit <= upper:
      return -3
    if (upper-lower) < 2.0:
      return -2
    if self.get_lock_state() != 0:
      return -1
    self.data_threshold_analysis(upper ,rslt)
    self.write_reg(T_UPPER_REGISTER ,rslt)
    rslt[0] = 0
    rslt[1] = 0
    self.data_threshold_analysis(lower ,rslt)
    self.write_reg(T_LOWER_REGISTER ,rslt)
    rslt[0] = 0
    rslt[1] = 0
    self.data_threshold_analysis(crit ,rslt)
    self.write_reg(T_CRIT_REGISTER ,rslt)    
    return 0


  def clear_interrupt(self):
    '''!
      @brief Clear interrupt, only used in interrupt mode, not work in other modes
    '''
    rslt = self.read_reg(CONFIG_REGISTER ,2)
    rslt[1] &= 0xDF
    rslt[1] |= 0x20
    self.write_reg(CONFIG_REGISTER ,rslt)

  def data_threshold_analysis(self ,value ,data):
    '''!
      @brief Threshold parsing
    '''
    symbol = 0
    decimals = 0
    integet = 0
    if value < 0.00001:
      symbol = 1
      value *= -1
    decimals = self.parsing_decimal(value)
    integer = int(value)
    data[0] |= symbol<<4
    data[0] |= integer>>4
    data[1] |= ((integer&0x0F)<<4)
    data[1] |= decimals

  def parsing_decimal(self ,value):
    '''!
      @brief Decimal parsing
    '''
    decimals = int(value*100) - (int(value))*100
    if decimals == 0:
      return 0x00
    elif decimals > 0 and decimals <= 25:
      return 0x04
    elif decimals > 25 and decimals <= 50:
      return 0x08
    else:
      return 0x0C


class DFRobot_MCP9808_I2C(DFRobot_MCP9808):
  def __init__(self ,bus ,addr):
    self.__addr = addr
    super(DFRobot_MCP9808_I2C, self).__init__(bus)

  def write_reg(self, reg, data):
    while 1:
      try:
        self.i2cbus.write_i2c_block_data(self.__addr ,reg ,data)
        return
      except:
        print("please check connect!")
        #os.system('i2cdetect -y 1')
        time.sleep(1)
        return

  def read_reg(self, reg ,len):
    try:
      rslt = self.i2cbus.read_i2c_block_data(self.__addr ,reg ,len)
      #print("please check connect!")
    except:
      rslt = -1
      print("please check connect!")
      time.sleep(1)
    return rslt
