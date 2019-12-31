#pragma once

#include "DHT.h"  //DHT11 or DHT22 Digital Humidity and Temperature sensor
#include "420Common.h"

class GrowBox;  //forward declaration

class DHTSensor : public Common
{
  public:
    DHTSensor(const __FlashStringHelper * Name, GrowBox * GBox, Settings::DHTSensorSettings * DefaultSettings);
    RollingAverage * Temp;
    RollingAverage * Humidity;
    //void websiteEvent_Load(__attribute__((unused)) char * url);
    void websiteEvent_Refresh(__attribute__((unused)) char * url);
    //void websiteEvent_Button(char * Button);
    //void websiteEvent_Field(char * Field);
    void refresh_Minute();  //Called when component should refresh its state
    void report();
    float getTemp();  
    float getHumidity();
    char * getTempText(bool IncludeUnits);  
    char * getHumidityText(bool IncludeUnits);
  
  private:
    GrowBox * GBox;
    DHT* Sensor; //Pointer declaration, points to null initially 
  
  protected:
};
