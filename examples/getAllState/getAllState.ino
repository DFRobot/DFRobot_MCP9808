/*!
 * @file getAllState.ino
 * @brief Get all the default config
 * @details Experimental phenomenon: serial print the sensor config status
 * @copyright Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license The MIT License (MIT)
 * @author ZhixinLiu(zhixin.liu@dfrobot.com)
 * @version V1.0
 * @date 2021-04-16
 * @url https://github.com/DFRobot/DFRobot_MCP9808
 */
#include "DFRobot_MCP9808.h"

/**
 * i2c address select, default to be MCP9808_ADDRESS_7, pin A2, A1 and A0 is at high level
 *    1 for high level, 0 for low level,  | A2 | A1 | A0 |
 *    MCP9808_ADDRESS_0        | 0  | 0  | 0  |    0x18
 *    MCP9808_ADDRESS_1        | 0  | 0  | 1  |    0x19
 *    MCP9808_ADDRESS_2        | 0  | 1  | 0  |    0x1A
 *    MCP9808_ADDRESS_3        | 0  | 1  | 1  |    0x1B
 *    MCP9808_ADDRESS_4        | 1  | 0  | 0  |    0x1D
 *    MCP9808_ADDRESS_5        | 1  | 0  | 1  |    0x1D
 *    MCP9808_ADDRESS_6        | 1  | 1  | 0  |    0x1E
 *    MCP9808_ADDRESS_7        | 1  | 1  | 1  |    0x1F
 */
#define I2C_ADDRESS  MCP9808_ADDRESS_7
DFRobot_MCP9808_I2C mcp9808(&Wire, I2C_ADDRESS);

void setup()
{
  Serial.begin(115200);
  while(!Serial);
  while(!mcp9808.begin()){
    Serial.println("begin failed!");
    delay(1000);
  } Serial.println("begin success!");

  /**
   * Get temp resolution
   *   RESOLUTION_0_5      The decimal part of the obtained temp is a multiple of 0.5     e.g. 0.5°C, 1.0°C, 1.5°C
   *   RESOLUTION_0_25     The decimal part of the obtained temp is a multiple of 0.25    e.g. 0.25°C, 0.50°C, 0.75°C
   *   RESOLUTION_0_125    The decimal part of the obtained temp is a multiple of 0.125   e.g. 0.125°C, 0.250°C, 0.375°C
   *   RESOLUTION_0_0625   The decimal part of the obtained temp is a multiple of 0.0625  e.g. 0.0625°C, 0.1250°C, 0.1875°C
   */
  switch(mcp9808.getResolution()){
    case RESOLUTION_0_5:
      Serial.println("Resolution: 0.5");
      break;
    case RESOLUTION_0_25:
      Serial.println("Resolution: 0.25");
      break;
    case RESOLUTION_0_125:
      Serial.println("Resolution: 0.125");
      break;
    case RESOLUTION_0_0625:
      Serial.println("Resolution: 0.0625");
      break;
    default:
      break;
  }

  /**
   * Get locking status, to determine if upper limit, lower limit, and threshold can be changed
   * Unlock only by power-off reset after locking
   *   CRIT_LOCK      Lock threshold value, which is not allowed to be changed
   *   WIN_LOCK       Lock upper and lower thresholds, which are not allowed to be changed
   *   CRIT_WIN_LOCK  Lock threshold and the window at the same time, upper limit, lower limit, and threshold are all not allowed to be changed
   *   NO_LOCK        No locking, threshold value, upper and lower limit are all can be changed
   */
  switch(mcp9808.getLockState()){
    case CRIT_LOCK:
      Serial.println("Locking mode: lock threshold");
      break;
    case WIN_LOCK:
      Serial.println("Locking mode: lock upper and lower limit");
      break;
    case CRIT_WIN_LOCK:
      Serial.println("Locking mode: lock threshold and the window at the same time");
      break;
    case NO_LOCK:
      Serial.println("Locking mode: no locking");
      break;
    default:
      break;
  }

  /**
   * Get the configured power mode, power-up mode and low-power mode
   *  POWER_UP_MODE      Wake-up mode, in this mode, the register can be accessed and the temp can be obtained normally.
   *  LOW_POWER_MODE     Temp measurement stop and the register can be read or written, but bus activity will cause higher power consumption.
   */
  if(mcp9808.getPowerMode()){
    Serial.println("Power mode: wake-up mode");
  }else{
    Serial.println("Power mode: sleep mode");
  }

  /**
   * Get alert hysteresis temp, hysteresis function is only applicable to reduce temp (from hot to cold), i.e, ALE level will be restored only when the temp reaches the value obtained by subtracting hysteresis temp from the upper limit/lower limit/threshold.
   *  HYSTERESIS_0_0   Temp hysteresis range +0.0℃
   *  HYSTERESIS_1_5   Temp hysteresis range +1.5℃
   *  HYSTERESIS_3_0   Temp hysteresis range +3.0℃
   *  HYSTERESIS_6_0   Temp hysteresis range +6.0℃
   */
  switch(mcp9808.getAlertHysteresis()){
    case HYSTERESIS_0_0:
      Serial.println("Temp hysteresis range: +0.0 C");
      break;
    case HYSTERESIS_1_5:
      Serial.println("Temp hysteresis range: +1.5 C");
      break;
    case HYSTERESIS_3_0:
      Serial.println("Temp hysteresis range: +3.0 C");
      break;
    case HYSTERESIS_6_0:
      Serial.println("Temp hysteresis range: +6.0 C");
      break;
    default:
      break;
  }

  /**
   * Get pin ALE polarity
   *  POLARITY_HIGH   pin ALE is active on high
   *  POLARITY_LOW    pin ALE is active on low
   */
  switch(mcp9808.getPolarityState()){
    case POLARITY_HIGH:
      Serial.println("Pin polarity: high polarity");
      break;
    case POLARITY_LOW:
      Serial.println("Pin polarity: low polarity");
      break;
    default:
      break;
  }

  /**
   * Get alert output mode
   *  COMPARATOR_OUTPUT_MODE  comparator output mode
   *  INTERRPUT_OUTPUT_MODE   interrupt output mode
   *  DISABLE_OUTPUT_MODE     disable output mode, ALE pin will be invalid
   */
  switch(mcp9808.getAlertOutputMode()){
    case DISABLE_OUTPUT_MODE:
      Serial.println("Output mode: disable output");
      break;
    case COMPARATOR_OUTPUT_MODE:
      Serial.println("Output mode: comparator output");
      break;
    case INTERRPUT_OUTPUT_MODE:
      Serial.println("Output mode: interrupt output");
      break;
    default:
      break;
  }

  /**
   * Get interrupt response mode
   *  UPPER_LOWER_CRIT_RESPONSE  respond to upper/lower limits and threshold
   *  ONLY_CRIT_RESPONSE         disable upper/lower limits response, only enable threshold response
   */
  switch(mcp9808.getAlertResponseMode()){
    case UPPER_LOWER_CRIT_RESPONSE:
      Serial.println("Response mode: upper/lower limit and threshold response");
      break;
    case ONLY_CRIT_RESPONSE:
      Serial.println("Response mode: threshold response");
      break;
    default:
      break;
  }
}

void loop()
{
  delay(1000);
}
