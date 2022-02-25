/*!
 * @file getTemperature.ino
 * @brief The example of obtaining temp normally
 * @details Experimental phenomenon: display the temp on the serial port, the measured temp varies with ambient temp
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
  if(mcp9808.setResolution(RESOLUTION_0_125)){
    Serial.println("Set temp resolution successfully!");
  }else{
    Serial.println("parameter error!");
  }
}

void loop()
{
  Serial.print("Temperature is ="); 
  Serial.print(mcp9808.getTemperature());
  Serial.println(" C");
  delay(1000);
}
