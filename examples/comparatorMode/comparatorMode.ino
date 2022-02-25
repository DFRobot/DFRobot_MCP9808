/*!
 * @file comparatorMode.ino
 * @brief Get the status of comparing the current temp with the upper/lower limit and threshold.
 * @details Experimental phenomenon: display temp and comparison status on the serial port
 * @details Experimental phenomenon: when the temp is above the upper limit, pin ALE level will switch,
 * @details Experimental phenomenon: when the temp is below the lower limit, pin ALE level will switch,
 * @details Experimental phenomenon: when the temp is above the critical temp, pin ALE level will switch,
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
   * Wake up sensor, sensor data can be obtained normally at this time
   */
  if(!mcp9808.wakeUpMode()){
    Serial.println("Register locked, Please unlock!");
  }else{
    Serial.println("Wake up sensor successfully, can read the temperature!");
  }

  /**
   * Set temp resolution
   *  RESOLUTION_0_5      The decimal part of the obtained temp is a multiple of 0.5     e.g. 0.5°C, 1.0°C, 1.5°C
   *  RESOLUTION_0_25     The decimal part of the obtained temp is a multiple of 0.25    e.g. 0.25°C, 0.50°C, 0.75°C
   *  RESOLUTION_0_125    The decimal part of the obtained temp is a multiple of 0.125   e.g. 0.125°C, 0.250°C, 0.375°C
   *  RESOLUTION_0_0625   The decimal part of the obtained temp is a multiple of 0.0625  e.g. 0.0625°C, 0.1250°C, 0.1875°C
   */
  if(mcp9808.setResolution(RESOLUTION_0_25)){
    Serial.println("Set temp resolution successfully!");
  }else{
    Serial.println("parameter error!");
  }

  /**
   * Set alert output mode, clearing interrupt is not required in comparator output mode, but required in interrupt mode
   *   COMPARATOR_OUTPUT_MODE           clearing interrupt is not required in comparator output mode, 
   *     For example: set pin ALE to be active on low, when the temp exceeds the upper limit alert temp, pin ALE level turns from high to low; when it's below the upper but above the lower limit, pin ALE level turns back to high
   *   INTERRPUT_OUTPUT_MODE            clearing interrupt is required in interrupt mode, when an alert occurs, the interrupt keep existing if it's not cleared, triggering interrupt mode indicates changing status. 
   *     For example, set lower threshold to be 20 degrees, upper threshold 25°C, and threshold 30°C, no interrupt occurs when temp is below 20°C; when temp exceeds 25°C, interrupt occurs and pin ALE jump, the interrupt should be cleared at the time and pin ALE is restored;
   *     in the special case, when current temp is above the critical temp of 30°C, the interrupt mode and clearing interrupt are both invalid, the interrupt mode will be restored only when the temp drops below 30°C.
   *   DISABLE_OUTPUT_MODE              After disabling output mode, no alert occurs and pin ALE is invalid
   */
  if(mcp9808.setAlertOutputMode(COMPARATOR_OUTPUT_MODE) == 0){
    Serial.println("Set alert output mode successfully!");
  }else{
    Serial.println("Register locked or parameter error!");
  }

  /**
   * Set pin ALE polarity, high polarity: pin ALE is active on high, the default is low level, which turns to high when alert occurs.
   *                    low polarity: pin ALE is active on low, the default is high level, which turns to low when alert occurs.
   *   POLARITY_HIGH         pin ALE is active on high
   *   POLARITY_LOW          pin ALE is active on low
   */
  if(mcp9808.setPolarity(POLARITY_LOW) == 0){
    Serial.println("Set pin ALE polarity successfully!");
  }else{
    Serial.println("Register locked or parameter error!");
  }

  /**
   * Set alert response mode, respond to upper & lower limits and threshold; or only respond to threshold, which is inapplicable to interrupt mode
   *  UPPER_LOWER_CRIT_RESPONSE          respond to upper/lower limits and threshold
   *  ONLY_CRIT_RESPONSE                 disable upper/lower limits response, only respond to threshold
   */
  if(mcp9808.setAlertResponseMode(UPPER_LOWER_CRIT_RESPONSE) == 0){
    Serial.println("Set response mode successfully!");
  }else{
    Serial.println("Register locked or parameter error!");
  }

  /**
   * Set threshold value, the upper and lower thresholds, and respond according to the configured interrupt mode, responding interruption when the temp is above the upper limit, below the lower limit and above the threshold
   *  crit:    Temp threshold, up to two decimal places, automatically processed to multiples of 0.25, range is from -40 to +125°C
   *  upper:   Upper limit temp, up to two decimal places, automatically processed to multiples of 0.25, range is from -40 to +125°C
   *  lower:   Lower limit temp, up to two decimal places, automatically processed to multiples of 0.25, range is from -40 to +125°C
   */
  if(mcp9808.setThreshold(/*crit*/35.0, /*upper*/30.0, /*lower*/20.0) == 0){
    Serial.println("Set temp of threshold, upper and lower limit successfully!");
  }else{
    Serial.println("Register locked or data error");
  }

  /**
   * Set alert temp hysteresis range, add a range to the upper limit, lower limit and threshold, hysteresis function is only applicable to reduce temperature (from hot to cold)
   * i.e, ALE level will be restored only when the temp reaches the value obtained by subtracting hysteresis temp from the upper limit.
   * For example: maximum temp is 30.0°C, hysteresis temp is +1.5°C, the current temp is 35°C and ALE level has flipped,
   *      the temp must reach 30-1.5=28.5°C before restoring ALE level
   *   HYSTERESIS_0_0        No hysteresis, respond when reaching the specified temp
   *   HYSTERESIS_1_5        1.5℃ lag from hot to cold
   *   HYSTERESIS_3_0        3.0℃ lag from hot to cold
   *   HYSTERESIS_6_0        6.0℃ lag from hot to cold
   */
  if(mcp9808.setAlertHysteresis(HYSTERESIS_1_5) == 0){
    Serial.println("Set temp hysteresis successfully!");
  }else{
    Serial.println("Register locked or parameter error!");
  }

  /**
   * Set lock mode or unlock, prevent wrong operations that may change upper limit, lower limit and threshold value,
   * and unlock only by power-off reset after locking
   *   CRIT_LOCK       Lock threshold value, which is not allowed to be changed
   *   WIN_LOCK        Lock upper and lower thresholds, which are not allowed to be changed
   *   CRIT_WIN_LOCK   Lock threshold value, upper and lower limit, which are not allowed to be changed
   *   NO_LOCK         No locking of threshold value, upper and lower limit
   */
  if(mcp9808.setLockState(NO_LOCK)){
    Serial.println("Set locking mode successfully!");
  }else{
    Serial.println("parameter error!");
  }
}

void loop()
{
  /**
   * Get the current comparator status and temp data, which works only in comparator mode
   * Struct storing the current data
   *   temperate        Current temp
   *   state            Character string of the comparator status, compare the current temp with the upper threshold, lower threshold, and threshold value
   */
  sComparator_t data = mcp9808.getComparatorState();
  Serial.print("temperature = ");
  Serial.print(data.temperature);
  Serial.println(" C");
  Serial.print("state is ");
  Serial.println(data.state);
  delay(1000);
  // if(mcp9808.sleepMode()){ Serial.println("sleep sensor success!");}  // Sensor enter sleep mode, fail to get the correct temp in sleep mode
}
