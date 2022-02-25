/*!
 * @file DFRobot_MCP9808.cpp
 * @brief Define the basic structure of class DFRobot_MCP9808
 * @copyright	Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license The MIT License (MIT)
 * @author ZhixinLiu(zhixin.liu@dfrobot.com)
 * @version V1.0
 * @date 2021-03-4
 * @url https://github.com/DFRobot/DFRobot_MCP9808
 */
#include "DFRobot_MCP9808.h"
/*
DFRobot_MCP9808::DFRobot_MCP9808(){}
DFRobot_MCP9808::~DFRobot_MCP9808(){}
*/

bool DFRobot_MCP9808::sensorInit(void)
{
  if(getDeviceID() == true && getManufacturerID() == true){
    return true;
  }else{
    return false;
  }
}

bool DFRobot_MCP9808::getManufacturerID(void)
{
  uint8_t rxbuf[20]={0x00};
  readData(MANUFACTURER_REGISTER ,rxbuf,2);
  if(rxbuf[1] == MANUFACTURER_ID){
    return true;
  }else{
    return false;
  }
}

bool DFRobot_MCP9808::getDeviceID(void)
{
  uint8_t rxbuf[20]={0x00};
  readData(DEVICE_REGISTER ,rxbuf ,2);
  if(rxbuf[0] == DEVICE_ID){
    return true;
  }else{
    return false;
  }
}

bool DFRobot_MCP9808::setResolution(uint8_t resolution)
{
  switch(resolution)
  {
    case RESOLUTION_0_5:
    case RESOLUTION_0_25:
    case RESOLUTION_0_125:
    case RESOLUTION_0_0625:
      writeData(RESOLUTION_REGISTER ,&resolution ,1);
      return true;
      break;
    default:
      return false;
      break;
  }
}

uint8_t DFRobot_MCP9808::getResolution(void)
{
  uint8_t rxbuf;
  readData(RESOLUTION_REGISTER ,&rxbuf ,1);
  return rxbuf&0x03;
}

float DFRobot_MCP9808::getTemperature(void)
{
  uint8_t rxbuf[20]={0x00};
  uint8_t temperatureHigh=0 ,temperatureLow=0;
  float temperature = 0.0;
  readData(TEMPERATURE_REGISTER ,rxbuf ,2);
  temperatureHigh = rxbuf[0];
  temperatureLow  = rxbuf[1];
  DBG(rxbuf[0],HEX);
  DBG(rxbuf[1],HEX);
  temperatureHigh &= 0x1F;
  if((temperatureHigh&0x10) == 0x10){  // temperature < 0
    temperatureHigh &= 0x0F;
    temperature = 256.0 - ((float)temperatureHigh*16.0 + (float)temperatureLow/16.0);
    return temperature;
  }else{                               // temperature > 0
    temperature = (float)temperatureHigh*16.0 + (float)temperatureLow/16.0;
    return temperature;
  }
}

uint8_t DFRobot_MCP9808::setAlertOutputMode(uint8_t mode)
{
  uint8_t rxbuf[20]={0x00};
  uint8_t state = getLockState();
  if(state != 0){
    DBG("The register is locked, please unlock!");
    return 0xFF;    // Lock mode cannot be configured
  }

  switch(mode)
  {
    case COMPARATOR_OUTPUT_MODE:
    case INTERRPUT_OUTPUT_MODE:
      setAlertEnable(ENABLE_ALERT);
      readData(CONFIG_REGISTER ,rxbuf ,2);
      rxbuf[1] &= 0xFE;
      rxbuf[1] |= mode;
      writeData(CONFIG_REGISTER ,rxbuf ,2);
      return 0x00;
      break;
    case DISABLE_OUTPUT_MODE:
      setAlertEnable(DISABLE_ALERT);
      return 0x00;
      break;
    default:
      DBG("parameter error!");
      return 0xFE;
      break;
  }
}

uint8_t DFRobot_MCP9808::getAlertOutputMode(void)
{
  uint8_t rxbuf[20]={0x00};
  if(getAlertEnableState() == DISABLE_ALERT){
    return DISABLE_OUTPUT_MODE;
  }else{
    readData(CONFIG_REGISTER ,rxbuf ,2);
    return (rxbuf[1]&0x01);
  }
}

sComparator_t DFRobot_MCP9808::getComparatorState(void)
{
  uint8_t rxbuf[20]={0x00};
  sComparator_t data;
  readData(TEMPERATURE_REGISTER ,rxbuf ,2);
  data.value = (rxbuf[0]&0xE0)>>5;
  data.temperature = getTemperature();
  if((data.value>>2)&0x01){
    data.state += " TA >= TCRIT ,";
  }else{
    data.state += " TA < TCRIT ,";
  }
  if((data.value>>1)&0x01){
    data.state += " TA > TUPPER ,";
  }else{
    data.state += " TA <= TUPPER ,";
  }
  if((data.value>>0)&0x01){
    data.state += " TA < TLOWER";
  }else{
    data.state += " TA >= TLOWER";
  }
  return data;
}

bool DFRobot_MCP9808::sleepMode(void)
{
  if(setPowerMode(LOW_POWER_MODE)){
    return true;
  }else{
    return false;
  }
}

bool DFRobot_MCP9808::wakeUpMode(void)
{
  if(setPowerMode(POWER_UP_MODE)){
    return true;
  }else{
    return false;
  }
}

bool DFRobot_MCP9808::setPowerMode(uint8_t mode)
{
  uint8_t rxbuf[20]={0x00};
  uint8_t state = getLockState();
  if(state != 0){
    DBG("The register is locked, please unlock!");
    return false;     // Lock mode cannot be configured
  }
  switch(mode)
  {
    case POWER_UP_MODE:
    case LOW_POWER_MODE:
      readData(CONFIG_REGISTER ,rxbuf ,2);
      rxbuf[0] &= 0x06;
      rxbuf[0] |= mode;
      writeData(CONFIG_REGISTER ,rxbuf ,2);
      return true;
      break;
  }
  return true;
}

bool DFRobot_MCP9808::getPowerMode(void)
{
  uint8_t rxbuf[20]={0x00};
  readData(CONFIG_REGISTER ,rxbuf ,2);
  if((rxbuf[0]&0x01) == POWER_UP_MODE){
    return true;
  }else{
    return false;
  }
}

bool DFRobot_MCP9808::setLockState(uint8_t lock)
{
  uint8_t rxbuf[20]={0x00};
  switch(lock)
  {
    case CRIT_LOCK:
    case WIN_LOCK:
    case CRIT_WIN_LOCK:
    case NO_LOCK:
      readData(CONFIG_REGISTER ,rxbuf ,2);
      rxbuf[1] &= 0x3F;
      rxbuf[1] |= lock;
      writeData(CONFIG_REGISTER ,rxbuf ,2);
      return true;
      break;
    default:
      return false;
      break;
  }
}

uint8_t DFRobot_MCP9808::getLockState(void)
{
  uint8_t rxbuf[20]={0x00};
  readData(CONFIG_REGISTER ,rxbuf ,2);
  return (rxbuf[1]&0xC0);
}

uint8_t DFRobot_MCP9808::setAlertHysteresis(uint8_t mode)
{
  uint8_t rxbuf[20]={0x00};
  uint8_t state = getLockState();
  if(state != 0){
    DBG("The register is locked, please unlock!");
    return 0xFF;                     // Lock mode cannot be configured
  }
  switch(mode)
  {
    case HYSTERESIS_0_0:
    case HYSTERESIS_1_5:
    case HYSTERESIS_3_0:
    case HYSTERESIS_6_0:
      readData(CONFIG_REGISTER ,rxbuf ,2);
      rxbuf[0] &= 0x01;
      rxbuf[0] |= mode;
      writeData(CONFIG_REGISTER ,rxbuf ,2);
      return 0x00;
    default:
      DBG("parameter error!");
      return 0xFE;
      break;
  }
}

uint8_t DFRobot_MCP9808::getAlertHysteresis(void)
{
  uint8_t rxbuf[20]={0x00};
  readData(CONFIG_REGISTER ,rxbuf ,2);
  return (rxbuf[0]&0x06);
}

uint8_t DFRobot_MCP9808::setAlertEnable(uint8_t mode)
{
  uint8_t rxbuf[20]={0x00};
  uint8_t state = getLockState();
  if(state != 0){
    DBG("The register is locked, please unlock!");
    return 0xFF;                     // Lock mode cannot be configured
  }
  switch(mode)
  {
    case ENABLE_ALERT:
    case DISABLE_ALERT:
      readData(CONFIG_REGISTER ,rxbuf ,2);
      rxbuf[1] &= 0xF7;
      rxbuf[1] |= mode;
      writeData(CONFIG_REGISTER ,rxbuf ,2);
      return 0x00;
      break;
    default:
      DBG("parameter error!");
      return 0xFE;
      break;
  }
}

uint8_t DFRobot_MCP9808::getAlertEnableState(void)
{
  uint8_t rxbuf[20]={0x00};
  readData(CONFIG_REGISTER ,rxbuf ,2);
  return (rxbuf[1]&0x08);
}



void DFRobot_MCP9808::clearInterrupt(void)
{
  uint8_t rxbuf[20]={0x00};
  readData(CONFIG_REGISTER ,rxbuf ,2);
  rxbuf[1] &= 0xDF;
  rxbuf[1] |= 0x20;
  writeData(CONFIG_REGISTER ,rxbuf ,2);
}

uint8_t DFRobot_MCP9808::setPolarity(uint8_t polarity)
{
  uint8_t rxbuf[20]={0x00};
  uint8_t state = getLockState();
  if(state != 0){
    DBG("The register is locked, please unlock!");
    return 0xFF;                     // Lock mode cannot be configured
  }
  switch(polarity)
  {
    case POLARITY_HIGH:
    case POLARITY_LOW:
      readData(CONFIG_REGISTER ,rxbuf ,2);
      rxbuf[1] &= 0xFD;
      rxbuf[1] |= polarity;
      writeData(CONFIG_REGISTER ,rxbuf ,2);
      return 0x00;
      break;
    default:
      DBG("parameter error!");
      return 0xFE;
      break;
  }
}

uint8_t DFRobot_MCP9808::getPolarityState(void)
{
  uint8_t rxbuf[20]={0x00};
  readData(CONFIG_REGISTER ,rxbuf ,2);
  return (rxbuf[1]&0x02);
}

uint8_t DFRobot_MCP9808::setAlertResponseMode(uint8_t mode)
{
  uint8_t rxbuf[20]={0x00};
  uint8_t state = getLockState();
  if(state != 0){
    DBG("The register is locked, please unlock!");
    return 0xFF;                     // Lock mode cannot be configured
  }
  switch(mode){
    case UPPER_LOWER_CRIT_RESPONSE:
    case ONLY_CRIT_RESPONSE:
      readData(CONFIG_REGISTER ,rxbuf ,2);
      rxbuf[1] &= 0xFB;
      rxbuf[1] |= mode;
      writeData(CONFIG_REGISTER ,rxbuf ,2);
      return 0x00;
      break;
    default:
      DBG("parameter error!");
      return 0xFE;
      break;
  }
}

uint8_t DFRobot_MCP9808::DFRobot_MCP9808::getAlertResponseMode(void)
{
  uint8_t rxbuf[20]={0x00};
  readData(CONFIG_REGISTER ,rxbuf ,2);
  return (rxbuf[1]&0x04);
}

uint8_t DFRobot_MCP9808::setThreshold(float crit, float upper, float lower)
{
  uint8_t rxbuf[20]={0x00};
  uint8_t state = getLockState();
  if(state != 0){
    DBG("The register is locked, please unlock!");
    return 0xFF;                     // Lock mode cannot be configured
  }
  if(crit < upper){
    DBG("crit < upper ");
    return 0xFE;
  }
  if((upper - lower) < 2.0){
    DBG("upper - lower < 2");
    return 0xFD;
  }
  memset(rxbuf,0,sizeof(rxbuf));
  dataThreshold(upper ,rxbuf);
  DBG(rxbuf[0],HEX);
  DBG(rxbuf[1],HEX);
  writeData(T_UPPER_REGISTER ,rxbuf ,2);

  memset(rxbuf,0,sizeof(rxbuf));
  dataThreshold(lower ,rxbuf);
  DBG(rxbuf[0],HEX);
  DBG(rxbuf[1],HEX);
  writeData(T_LOWER_REGISTER ,rxbuf ,2);
  
  memset(rxbuf,0,sizeof(rxbuf));
  dataThreshold(crit ,rxbuf);
  DBG(rxbuf[0],HEX);
  DBG(rxbuf[1],HEX);
  writeData(T_CRIT_REGISTER ,rxbuf ,2);
  return 0x00;
}


void DFRobot_MCP9808::dataThreshold(float value ,uint8_t *data)
{
  uint8_t symbol = 0;
  uint8_t decimals = 0;
  uint8_t integer = 0;
  if(value < 0.0){
    symbol = 1;
    value *= -1;
  }
  decimals = parsingDecimal(value);
  integer = (uint8_t)value;
  data[0] |= (symbol<<4);            // Sign position bit
  data[0] |= (integer>>4);           // high 4 bit
  data[1] |= ((integer&0x0F)<<4);
  data[1] |= (decimals);
}

uint8_t DFRobot_MCP9808::parsingDecimal(float value)
{
  uint8_t decimals = uint8_t((uint16_t)(value*100) - ((uint16_t)value)*100);
  if(decimals == 0){
    decimals = 0x00;
  }else if(decimals > 0 && decimals <= 25){
    decimals = 0x04;
  }else if(decimals > 25 && decimals < 50){
    decimals = 0x08;
  }else{
    decimals = 0x0C;
  }
  return decimals;
}

DFRobot_MCP9808_I2C::DFRobot_MCP9808_I2C(TwoWire *pWire, uint8_t addr)
{
  _pWire = pWire;
  this->_I2C_addr = addr;
}

bool DFRobot_MCP9808_I2C::begin(void)
{
  _pWire->begin();
  _pWire->beginTransmission(_I2C_addr);
  
  if(_pWire->endTransmission() == 0){
    if(sensorInit()==true){
      return true;
    }else{
      DBG("manufacturer ID or Device ID error!");
      return false;
    }
  }else{
    DBG("I2C connect error");
    return false;
  }
}

void DFRobot_MCP9808_I2C::writeData(uint8_t reg ,uint8_t *data ,uint8_t len)
{
  _pWire->beginTransmission(this->_I2C_addr);
  _pWire->write(reg);
  for(uint8_t i = 0; i < len; i++)
    _pWire->write(data[i]);
  _pWire->endTransmission();
}

uint8_t DFRobot_MCP9808_I2C::readData(uint8_t reg ,uint8_t *data ,uint8_t len)
{
  int i=0;
  _pWire->beginTransmission(this->_I2C_addr);
  _pWire->write(reg);
  _pWire->endTransmission();
  _pWire->requestFrom((uint8_t)this->_I2C_addr,(uint8_t)len);
  while (_pWire->available()){
    data[i++]=_pWire->read();
  }
  return 0;
}