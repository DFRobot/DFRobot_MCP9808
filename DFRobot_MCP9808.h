/*!
 * @file DFRobot_MCP9808.h
 * @brief Define the basic structure of class DFRobot_MCP9808
 * @copyright	Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license The MIT License (MIT)
 * @author ZhixinLiu(zhixin.liu@dfrobot.com)
 * @version V1.0
 * @date 2021-03-4
 * @url https://github.com/DFRobot/DFRobot_MCP9808
 */
#ifndef __DFROBOT_MCP9808_H__
#define __DFROBOT_MCP9808_H__
#include <Arduino.h>
#include <Wire.h>

//#define ENABLE_DBG
//#define NORMAL_ENABLE

#ifdef ENABLE_DBG
  #ifdef NORMAL_ENABLE
    #define DBG(...) {Serial.println(__VA_ARGS__);}
  #else
    #define DBG(...) {Serial.print("[");Serial.print(__FUNCTION__); Serial.print("(): "); Serial.print(__LINE__); Serial.print(" ] "); Serial.println(__VA_ARGS__);}
  #endif
#else
  #define DBG(...)
#endif

#define MCP9808_ADDRESS_REGISTER    (0x00)

#define FRU_REGISTER                (0x00 & 0x0F)
#define CONFIG_REGISTER             (0x01 & 0x0F)
#define T_UPPER_REGISTER            (0x02 & 0x0F)
#define T_LOWER_REGISTER            (0x03 & 0x0F)
#define T_CRIT_REGISTER             (0x04 & 0x0F)
#define TEMPERATURE_REGISTER        (0x05 & 0x0F)
#define MANUFACTURER_REGISTER       (0x06 & 0x0F)
#define DEVICE_REGISTER             (0x07 & 0x0F)
#define RESOLUTION_REGISTER         (0x08 & 0x0F)

#define MANUFACTURER_ID             (0x54)
#define DEVICE_ID                   (0x04)

#define RESOLUTION_0_5              (0x00 & 0x03)
#define RESOLUTION_0_25             (0x01 & 0x03)
#define RESOLUTION_0_125            (0x02 & 0x03)
#define RESOLUTION_0_0625           (0x03 & 0x03)          ///< default resolutivn

#define COMPARATOR_OUTPUT_MODE      0x00                   ///< default comparator mode
#define INTERRPUT_OUTPUT_MODE       0x01
#define DISABLE_OUTPUT_MODE         0x02

#define CRIT_LOCK                   0x80
#define WIN_LOCK                    0x40
#define CRIT_WIN_LOCK               0xC0
#define NO_LOCK                     0x00                   ///< default no lock

#define POWER_UP_MODE               0x00                   ///< dafault power up
#define LOW_POWER_MODE              0x01

#define HYSTERESIS_0_0              0x00                   ///< default hysteresis
#define HYSTERESIS_1_5              0x02
#define HYSTERESIS_3_0              0x04
#define HYSTERESIS_6_0              0x06

#define POLARITY_HIGH               0x02
#define POLARITY_LOW                0x00                   ///< default active low

#define ENABLE_ALERT                0x08
#define DISABLE_ALERT               0x00                   ///< default disable

#define ONLY_CRIT_RESPONSE          0x40
#define UPPER_LOWER_CRIT_RESPONSE   0x00                   ///< dafault output mode

#define MCP9808_ADDRESS_0               0x18
#define MCP9808_ADDRESS_1               0x19
#define MCP9808_ADDRESS_2               0x1a
#define MCP9808_ADDRESS_3               0x1b
#define MCP9808_ADDRESS_4               0x1c
#define MCP9808_ADDRESS_5               0x1d
#define MCP9808_ADDRESS_6               0x1e
#define MCP9808_ADDRESS_7               0x1f

/**
 * @struct sComparator_t
 * @brief Return message character string and data
 */
typedef struct{
  float temperature;            /**< temp data */
  uint8_t value;                /**< The status value of comparing the current temp with comparator, upper and lower temp respectively */
  String  state;                /**< The character string of comparing the current temp with comparator, upper and lower temp respectively */
} sComparator_t;

class DFRobot_MCP9808{
public:
  DFRobot_MCP9808(){};
  ~DFRobot_MCP9808(){};

  /**
   * @fn sleepMode
   * @brief Sensor sleep mode, lower power consumption, failed to get sensor temp at this time
   * @return bool type, indicate the returned status
   * @retval true is set successfully
   * @retval false is register lock is not allowed to be modified
   */
  bool sleepMode(void);

  /**
   * @fn wakeUpMode
   * @brief Sensor waking up mode, sensor data can be obtained normally at this time
   * @return bool type, indicate the returned status
   * @retval true is set successfully
   * @retval false is register lock is not allowed to be modified
   */
  bool wakeUpMode(void);

  /**
   * @fn wakeUpMode
   * @brief Get the configured power mode
   * @return bool type, indicate the returned status
   * @retval true is waking up mode
   * @retval false is sleep mode
   */
  bool getPowerMode(void);

  /**
   * @fn setResolution
   * @brief Set resolution of the temperature sensor, the accuracy of the acquired temp is different in different resolution
   * @param resolution
   * @n     RESOLUTION_0_5      The decimal part of the obtained temp is a multiple of 0.5     e.g. 0.5°C, 1.0°C, 1.5°C
   * @n     RESOLUTION_0_25     The decimal part of the obtained temp is a multiple of 0.25    e.g. 0.25°C, 0.50°C, 0.75°C
   * @n     RESOLUTION_0_125    The decimal part of the obtained temp is a multiple of 0.125   e.g. 0.125°C, 0.250°C, 0.375°C
   * @n     RESOLUTION_0_0625   The decimal part of the obtained temp is a multiple of 0.0625  e.g. 0.0625°C, 0.1250°C, 0.1875°C
   * @return The set status
   * @retval true is set successfully
   * @retval false is set parameter error
   */
  bool setResolution(uint8_t resolution);

  /**
   * @fn getResolution
   * @brief Get resolution of the temperature sensor, the accuracy of the acquired temp is different in different resolution
   * @return resolution
   * @retval RESOLUTION_0_5      The decimal part of the obtained temp is a multiple of 0.5     e.g. 0.5°C, 1.0°C, 1.5°C
   * @retval RESOLUTION_0_25     The decimal part of the obtained temp is a multiple of 0.25    e.g. 0.25°C, 0.50°C, 0.75°C
   * @retval RESOLUTION_0_125    The decimal part of the obtained temp is a multiple of 0.125   e.g. 0.125°C, 0.250°C, 0.375°C
   * @retval RESOLUTION_0_0625   The decimal part of the obtained temp is a multiple of 0.0625  e.g. 0.0625°C, 0.1250°C, 0.1875°C
   */
  uint8_t getResolution(void);

  /**
   * @fn getTemperature
   * @brief Get the current ambient temp, note that different temp accuracy can be obtained by setting different resolution
   * @return The temp value is a floating point, default to reserve two decimal places, unit is °C
   */
  float getTemperature(void);

  /**
   * @fn getComparatorState
   * @brief Get the current comparator status and temp data, which works only in comparator mode
   * @return Struct for storing the current data
   * @n      temperate         Current temp
   * @n      state             Character string of the comparator status, compare the current temp with the upper threshold, lower threshold, and threshold value
   * @n      value             Value of the comparator status, explained as follows
   * @n      TA for current temp, TCRIT for critical temp, TUPPER for maximum temp, TLOWER for minimum temp
   * @n      1 for TA ≥ TCRIT, TA > TUPPER, TA < TLOWER
   * @n      0 for TA < TCRIT, TA ≤ TUPPER, TA ≥ TLOWER
   * @n      ------------------------------------
   * @n      | bit7 ~ bit3 | bit2 | bit1 | bit0 |
   * @n      ------------------------------------
   * @n      |  reserved   |  0   |  0   |  0   |
   * @n      ------------------------------------
   */
  sComparator_t getComparatorState(void);

  /**
   * @fn setLockState
   * @brief Set lock mode or unlock, prevent wrong operations that may change upper limit, lower limit and threshold value, and unlock only by power-off reset after locking
   * @param lock
   * @n     CRIT_LOCK        Lock threshold value, which is not allowed to be changed
   * @n     WIN_LOCK         Lock upper and lower thresholds, which are not allowed to be changed
   * @n     CRIT_WIN_LOCK    Lock threshold value, upper and lower limit, which are not allowed to be changed
   * @n     NO_LOCK          No locking of threshold value, upper and lower limit
   * @return bool type return the set status
   * @retval true  is set successfully
   * @retval false is set parameter error
   */
  bool setLockState(uint8_t lock);

  /**
   * @fn getLockState
   * @brief Get locking status, to determine if upper limit, lower limit, and threshold can be changed
   * @return Locking status
   * @retval CRIT_LOCK        Lock threshold value, which is not allowed to be changed
   * @retval WIN_LOCK         Lock upper and lower thresholds, which are not allowed to be changed
   * @retval CRIT_WIN_LOCK    Lock threshold and the window at the same time, upper limit, lower limit, and threshold are all not allowed to be changed
   * @retval NO_LOCK          No locking, threshold value, upper and lower limit are all can be changed
   */
  uint8_t getLockState(void);

  /**
   * @fn setAlertHysteresis
   * @brief Set alert temp hysteresis range, add a range to the upper limit, lower limit and threshold, hysteresis function is only applicable to reduce temperature(from hot to cold),
   * @details i.e, ALE level will be restored only when the temp reaches the value obtained by subtracting hysteresis temp from the upper limit/lower limit/threshold
   * @details  For example: maximum temp is 30.0°C, hysteresis temp is +1.5°C, the current temp is 35°C and ALE level has flipped, 
   * @details  the temp must reach 30-1.5=28.5°C before restoring ALE level
   * @param mode
   * @n     HYSTERESIS_0_0        No hysteresis, there is respond when reaching the specified temp
   * @n     HYSTERESIS_1_5        1.5℃ lag from hot to cold
   * @n     HYSTERESIS_3_0        3.0℃ lag from hot to cold
   * @n     HYSTERESIS_6_0        6.0℃ lag from hot to cold
   * @return uint8_t type
   * @retval 0x00 is set successfully
   * @retval 0xFE is set parameter error
   * @retval 0xFF is register lock, not allowed to be modified
   */
  uint8_t setAlertHysteresis(uint8_t mode);

  /**
   * @fn getAlertHysteresis
   * @brief Get hysteresis temp
   * @return hysteresis type
   * @retval HYSTERESIS_0_0        Temp hysteresis range +0.0℃
   * @retval HYSTERESIS_1_5        Temp hysteresis range +1.5℃
   * @retval HYSTERESIS_3_0        Temp hysteresis range +3.0℃
   * @retval HYSTERESIS_6_0        Temp hysteresis range +6.0℃
   */
  uint8_t getAlertHysteresis(void);

  /**
   * @fn setPolarity
   * @brief Set pin ALE polarity, high polarity: pin ALE is active on high, the default is low level, which turns to high when alert occurs.
   * @details                 low polarity: pin ALE is active on low, the default is high level, which turns to low when alert occurs.
   * @param polarity
   * @n     POLARITY_HIGH          pin ALE is active on high
   * @n     POLARITY_LOW           pin ALE is active on low
   * @return state
   * @retval 0x00 is set successfully
   * @retval 0xFE is set parameter error
   * @retval 0xFF is register lock, not allowed to be modified
   */
  uint8_t setPolarity(uint8_t polarity);
  
  /**
   * @fn getPolarityState
   * @brief Get pin ALE polarity, high polarity: pin ALE is active on high, the default is low level, which turns to high when alert occurs.
   * @details                     low polarity: pin ALE is active on low, the default is high level, which turns to low when alert occurs.
   * @return polarity
   * @retval POLARITY_HIGH          pin ALE is active on high
   * @retval POLARITY_LOW           pin ALE is active on low
   */
  uint8_t getPolarityState(void);

  /**
   * @fn setAlertOutputMode
   * @brief Set alert output mode, clearing interrupt is not required in comparator output mode, but required in interrupt mode
   * @param mode:
   * @n     COMPARATOR_OUTPUT_MODE           clearing interrupt is not required in comparator output mode, 
   * @n         For example: set pin ALE to be active on low, when the temp exceeds the upper limit alert temp, pin ALE level turns from high to low; when it's below the upper but above the lower limit, pin ALE level turns back to high
   * @n     INTERRPUT_OUTPUT_MODE            clearing interrupt is required in interrupt mode, when an alert occurs, the interrupt keep existing if it's not cleared, triggering interrupt mode indicates one status to another, 
   * @n         For example, set lower threshold to be 20°C, upper threshold 25°C, and threshold 30°C, no interrupt occurs when temp is below 20°C; when temp exceeds 25°C, interrupt occurs and pin ALE jump, the interrupt should be cleared at the time and pin ALE is restored; in the special case, when pin ALE is above the critical temp of 30°C, the interrupt mode and clearing interrupt are both invalid, the interrupt mode will be restored only when the temp drops below 30 degrees.
   * @n     DISABLE_OUTPUT_MODE              After disabling output mode, no alert occurs and pin ALE is invalid
   * @return state
   * @retval 0x00 is set successfully
   * @retval 0xFE is set parameter error
   * @retval 0xFF is register lock, not allowed to be modified
   */
  uint8_t setAlertOutputMode(uint8_t mode);

  /**
   * @fn getAlertOutputMode
   * @brief Get alert output mode
   * @return mode
   * @retval COMPARATOR_OUTPUT_MODE           Comparator output mode
   * @retval INTERRPUT_OUTPUT_MODE            Interrupt output mode
   * @retval DISABLE_OUTPUT_MODE              Disable output mode
   */
  uint8_t getAlertOutputMode(void);

  /**
   * @fn setAlertResponseMode
   * @brief Set alert response mode, respond to upper & lower limits and threshold; or only respond to threshold, which is inapplicable to interrupt mode
   * @param mode
   * @n     UPPER_LOWER_CRIT_RESPONSE         respond to upper/lower limits and threshold 
   * @n     ONLY_CRIT_RESPONSE                disable upper/lower limits response, only enable threshold response
   * @return state
   * @retval 0x00 is set successfully
   * @retval 0xFE is set parameter error
   * @retval 0xFF is register lock, not allowed to be modified
   */
  uint8_t setAlertResponseMode(uint8_t mode);

  /**
   * @fn getAlertResponseMode
   * @brief Get interrupt response mode
   * @return mode
   * @retval UPPER_LOWER_CRIT_RESPONSE         respond to upper/lower limits and threshold
   * @retval ONLY_CRIT_RESPONSE                disable upper/lower limits response, only enable threshold response
   */
  uint8_t getAlertResponseMode(void);

  /**
   * @fn setThreshold
   * @brief Set threshold value, the upper and lower thresholds, and respond according to the configured interrupt mode
   * @details Responding interrupt when the temp is above the upper limit, below the lower limit and above the threshold
   * @details The critical temp must be above the upper limit
   * @details the upper limit temp must be 2℃ higher than the lower limit temp 
   * @param crit   Temp threshold, up to two decimal places, automatically processed to multiples of 0.25, range is from -40 to +125℃
   * @param upper  Upper limit temp, up to two decimal places, automatically processed to multiples of 0.25, range is from -40 to +125℃
   * @param lower  Lower limit temp, up to two decimal places, automatically processed to multiples of 0.25, range is from -40 to +125℃
   * @return state
   * @retval 0x00 is set successfully
   * @retval 0xFD  Upper limit temp is below lower limit, or (upper limit temp - lower limit temp < 2)
   * @retval 0xFE  Upper limit temp is below threshold
   * @retval 0xFF is register lock, not allowed to be modified
   */
  uint8_t setThreshold(float crit, float upper, float lower);

  /**
   * @fn clearInterrupt
   * @brief Clear interrupt, only used in interrupt mode, not work in other modes
   */
  void clearInterrupt(void);

protected:
  /**
   * @fn sensorInit
   * @brief Initialize sensor, comtrast sensor chip id with manufacturer id
   * @return state
   * @retval true  is init successfully
   * @retval flase is sensor chip id and manufacturer id error
   */
  bool sensorInit(void);

  /**
   * @fn setPowerMode
   * @brief Set power mode, power up mode: in this mode, the register can be accessed and the temp can be obtained normally.
   * @details            Low power mode: temp measurement stop and the register can be read or written, but bus activity will cause higher power consumption
   * @param mode
   * @n     POWER_UP_MODE         power up mode
   * @n     LOW_POWER_MODE        low power mode
   * @return The set status
   * @retval true  is set success
   * @retval false is register lock, not allowed to be modified
   */
  bool setPowerMode(uint8_t mode);

  /**
   * @fn setAlertEnable
   * @brief Enable or disable alert mode, after enabling it, pin ALE will jump when satisfying the alert condition; if disabling it, pin ALE has no response.
   * @param mode
   * @n     ENABLE_ALERT           after enabling alert mode, pin ALE will jump when satisfying the alert condition
   * @n     DISABLE_ALERT          when disabling alert mode, pin ALE has no response
   * @return state
   * @retval 0x00 is set successfully
   * @retval 0xFE is set parameter error
   * @retval 0xFF is register lock, not allowed to be modified
   */
  uint8_t setAlertEnable(uint8_t mode);

  /**
   * @fn getAlertEnableState
   * @brief Get the alert mode status, enable alert mode or disable it
   * @return mode
   * @retval ENABLE_ALERT            enable alert mode
   * @retval DISABLE_ALERT           disable alert mode
   */
  uint8_t getAlertEnableState(void);

  /**
   * @fn parsingDecimal
   * @brief Parse floating point, used to set the floating point of upper/lower limit and threshold
   * @return Binary floating point
   */
  uint8_t parsingDecimal(float value);

  /**
   * @fn getManufacturerID
   * @brief get manufacturer id
   * @return manufacturer id
   * @retval true                manufacturer id is true
   * @retval false               manufacturer id is false
   */
  bool getManufacturerID(void);

  /**
   * @fn getDeviceID
   * @brief get device id
   * @return device id
   * @retval true                device id is true
   * @retval false               device id is false
   */
  bool getDeviceID(void);

  /**
   * @fn dataThreshold
   * @brief Parse the integer portion of the threshold, and place it in the specified position
   * @param value The threshold data to be placed
   * @param data  The address to place
   */
  void dataThreshold(float value ,uint8_t *data);
  
  virtual uint8_t readData (uint8_t reg ,uint8_t *data ,uint8_t len)=0;
  virtual void    writeData(uint8_t reg ,uint8_t *data ,uint8_t len)=0;
};

class DFRobot_MCP9808_I2C:public DFRobot_MCP9808{
public:
  DFRobot_MCP9808_I2C(TwoWire *pWire=&Wire ,uint8_t addr=0x30);
  ~DFRobot_MCP9808_I2C(){};
  void writeData(uint8_t reg,uint8_t *data,uint8_t len);
  uint8_t readData(uint8_t reg,uint8_t *data,uint8_t len);
  bool begin(void);
private:
  TwoWire *_pWire;
  uint8_t _I2C_addr;
};
#endif
