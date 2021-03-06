#include "WaterTempSensor.h"

WaterTempSensor::WaterTempSensor(const __FlashStringHelper *Name, Module *Parent, Settings::WaterTempSensorSettings *DefaultSettings) : Common(Name)
{ ///constructor
  this->Parent = Parent;
  ///pinMode(*Pin, INPUT);
  Temp = 0.0;
  TempSensorWire = new OneWire(*(&DefaultSettings->Pin)); ///Reservoir waterproof temperature sensor (DS18B20)
  TempSensor = new DallasTemperature(TempSensorWire);     ///Reservoir waterproof temperature sensor (DS18B20)
  TempSensor->begin();
  Parent->addToReportQueue(this);          ///Subscribing to the report queue: Calls the report() method
  Parent->addToRefreshQueue_Minute(this); 
  logToSerials(F("WaterTempSensor object created"), true, 1);
}

void WaterTempSensor::refresh_Minute()
{
  if (*Debug)
    Common::refresh_Minute();  
  readSensor();
}

void WaterTempSensor::report()
{
  Common::report();
  memset(&LongMessage[0], 0, sizeof(LongMessage)); ///clear variable
  strcat_P(LongMessage, (PGM_P)F("Temp:"));
  strcat(LongMessage, getTempText(true));
  logToSerials(&LongMessage, true, 1);
}

void WaterTempSensor::readSensor(){
  TempSensor->requestTemperatures();
  if (*Metric)
  {
    Temp = TempSensor->getTempCByIndex(0);
  }
  else
  {
    Temp = TempSensor->getTempFByIndex(0);
  }
}

float WaterTempSensor::getTemp()
{
  return Temp;
}

char *WaterTempSensor::getTempText(bool IncludeUnits)
{
  if (IncludeUnits)
    return tempToText(Temp);
  else
    return toText(Temp);
}