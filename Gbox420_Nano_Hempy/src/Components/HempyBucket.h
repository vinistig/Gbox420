#pragma once
#include "420Common.h"
#include "420Module.h"
#include "Sound.h"
#include "WeightSensor.h"
#include "WaterPump.h"

///UNDER DEVELOPMENT - Class representing a single hempy Bucket
///Weight sensor measures the bucket weight and watering is based on the measured weight

class HempyBucket : virtual public Common
{
public:
  HempyBucket(const __FlashStringHelper *Name, Module *Parent, Settings::HempyBucketSettings *DefaultSettings, WeightSensor *BucketWeightSensor, WaterPump *BucketPump);
  void refresh_Minute();
  void refresh_Sec();
  void report();
  void waterNow(bool UserRequest = false);  ///Turn on water pump, run until StopWeight is reached
  
  void checkRelay(); ///Sets the relay controlling the pump
  void checkBucketWeight();
  void setStartWeight(float Weight);
  void setStopWeight(float Weight);
  float getStopWeight();
  float getStartWeight(); 
  char *getStopWeightText(bool IncludeUnits);
  char *getStartWeightText(bool IncludeUnits); 

private:  

protected:
  Module *Parent;
  WeightSensor *BucketWeightSensor;  ///Weight sensor to monitor the Hempy Bucket's weight, used to figure out when to start and stop watering
  WaterPump *BucketPump;  ///Weight sensor to monitor the Hempy Bucket's weight, used to figure out when to start and stop watering
  float *StartWeight;  ///Start watering below this weight
  float *StopWeight; ///Stop watering above this weight
};