/*!
 * @file interruptMode.ino
 * @brief The example of the upper, lower, and threshold temp triggering an interrupt
 * @details Experimental phenomenon: display the temp on the serial port, the measured temp varies with ambient temp
 * @details Experimental phenomenon: interrupt occurs at IO when the temp status changes, for example, no interrupt occurs if the temp is between the upper limit and lower limit, or has been lower than the lower or higher than the upper limit
 * @details Experimental phenomenon: when the temp status turns from being below the lower limit to above it, interrupt occurs and needs to be cleared,
 * @details Experimental phenomenon: after clearing the interrupt, pin ALE level is restored; when the temp is above the threshold, clearing interrupt is invalid(pin ALE level is not restored)
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

volatile uint8_t interruptFlag = 0;
void myInterrupt(void)
{
  interruptFlag = 1;   // interrupt flag
}

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
   *   INTERRPUT_OUTPUT_MODE            clearing interrupt is required in interrupt mode, when an alert occurs, the interrupt keep existing if it's not cleared, triggering interrupt mode indicates one status to another, 
   *     For example, set lower threshold to be 20°C, upper threshold 25°C, and threshold 30°C, no interrupt occurs when temp is below 20°C; when temp exceeds 25°C, interrupt occurs and pin ALE jump, the interrupt should be cleared at the time and pin ALE is restored,
   *     In the special case, when pin ALE is above the critical temp of 30°C, the interrupt mode and clearing interrupt are both invalid, the interrupt mode will be restored only when the temp drops below 30°C.
   *   DISABLE_OUTPUT_MODE              After disabling output mode, no alert occurs and pin ALE is invalid
   */
  if(mcp9808.setAlertOutputMode(INTERRPUT_OUTPUT_MODE) == 0){
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
   *  ONLY_CRIT_RESPONSE                 disable upper/lower limits response, only enable threshold response
   */
  if(mcp9808.setAlertResponseMode(UPPER_LOWER_CRIT_RESPONSE) == 0){
    Serial.println("Set response mode successfully!");
  }else{
    Serial.println("Register locked or parameter error!");
  }

  /**
   * Set threshold value, the upper and lower thresholds, and respond according to the configured interrupt mode
   *  crit:    Temp threshold, up to two decimal places, automatically processed to multiples of 0.25, range is from -40 to +125°C
   *  upper:   Upper limit temp, up to two decimal places, automatically processed to multiples of 0.25, range is from -40 to +125°C
   *  lower:   Lower limit temp, up to two decimal places, automatically processed to multiples of 0.25, range is from -40 to +125°C
   */
  if(mcp9808.setThreshold(35.0, 30.0, 20.0) == 0){
    Serial.println("Set temp of threshold, upper and lower limit successfully!");
  }else{
    Serial.println("Register locked or data error");
  }

  /**
   * Set alert temp hysteresis range, add a range to the upper limit, lower limit and threshold, hysteresis function is only applicable to reduce temperature(from hot to cold),
   * i.e, ALE level will be restored only when the temp reaches the value obtained by subtracting hysteresis temp from the upper limit
   * For example: maximum temp is 30.0°C, hysteresis temp is +1.5°C, the current temp is 35°C and ALE level has flipped,
   *      the temp must reach 30-1.5(28.5) degrees before restoring ALE level
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
   * Set lock mode or unlock, prevent wrong operations that may change upper limit, lower limit and threshold value, and unlock only by power-off reset after locking
   * unlock only by power-off reset after locking
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

  mcp9808.clearInterrupt();       //The temp status is unknown at the first time, so clear the interrupt
  #if defined(ESP32) || defined(ESP8266)||defined(ARDUINO_SAM_ZERO)
  /**
   * Select according to the set pin ALE polarity
   *  INPUT_PULLUP    Set polarity to low level, set pin 2 to pull-up input
   *  INPUT_PULLDOWN  Set polarity to high level, set pin 2 to pull-down input
   * interput io
   *  All pins can be used. Pin 13 is recommended
   */
  pinMode(/*Pin */13, INPUT_PULLUP);
  attachInterrupt(/*interput io*/13, myInterrupt, FALLING);
  Serial.println("use esp type");
  #else
  /**
   * ---------------------------------------------------------------------------------------
   * |                                        |    Pin       | 2  | 3  |                   |
   * |    Uno, Nano, Mini, other 328-based    |--------------------------------------------|
   * |                                        | Interrupt No | 0  | 1  |                   |
   * |-------------------------------------------------------------------------------------|
   * |                                        |    Pin       | 2  | 3  | 21 | 20 | 19 | 18 |
   * |               Mega2560                 |--------------------------------------------|
   * |                                        | Interrupt No | 0  | 1  | 2  | 3  | 4  | 5  |
   * |-------------------------------------------------------------------------------------|
   * |                                        |    Pin       | 3  | 2  | 0  | 1  | 7  |    |
   * |    Leonardo, other 32u4-based          |--------------------------------------------|
   * |                                        | Interrupt No | 0  | 1  | 2  | 3  | 4  |    |
   * |--------------------------------------------------------------------------------------
   */
  /**
   *                       The Correspondence Table of micro:bit Interrupt Pins And Terminal Numbers
   * ---------------------------------------------------------------------------------------------------------------------------------------------
   * |             micro:bit                       | DigitalPin |P0-P20 can be used as an external interrupt                                     |
   * |  (When using as an external interrupt,      |---------------------------------------------------------------------------------------------|
   * |no need to set it to input mode with pinMode)|Interrupt No|Interrupt number is a pin digital value, such as P0 interrupt number 0, P1 is 1 |
   * |-------------------------------------------------------------------------------------------------------------------------------------------|
   */
  pinMode(/*Pin */2 ,INPUT_PULLUP);

  /**
   * Set the pin to interrupt mode
   * Open the external interrupt 0, connect INT1/2 to the digital pin of the main control:
   *  function
   *   callback function
   *  state
   *   FALLING    When the pin level goes from high to low, the interrupt occur, and enter interrupt function
   *   RISING     When the pin level goes from low to high, the interrupt occur, and enter interrupt function
   */
  attachInterrupt(/*Interrupt No*/0, /*function*/myInterrupt, /*state*/FALLING);
  #endif
}

void loop()
{
  /**
   * Clear the alert interrupt when an interrupt is generated by ALE io port
   * Clear the interrupt generated by the upper/lower limit and threshold temp, the threshold interrupt can't be cleared
   */
  if(interruptFlag == 1){
    mcp9808.clearInterrupt();
    delay(1000);
    float temperature = mcp9808.getTemperature();
    Serial.print("Temperature is =");
    Serial.print(temperature);
    Serial.print(" C ");
    Serial.println("The temperature state has changed");
    interruptFlag = 0;
  }
    float temperature = mcp9808.getTemperature();
    Serial.print("Temperature is =");
    Serial.print(temperature);
    Serial.println(" C ");
  delay(1000);
}
