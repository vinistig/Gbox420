#pragma once

#include "420Common.h"
#include "420Module.h"
#include "Sound.h"

enum PumpState {DISABLED,IDLE, PRIME, RUNNING, BLOWOFF, MIXING};

class WaterPump : virtual public Common
{
public:
  
  WaterPump(const __FlashStringHelper *Name, Module *Parent, Settings::WaterPumpSettings *DefaultSettings); 
  void report();
  void refresh_Sec();
  PumpState State = IDLE;  
  void UpdateState(PumpState NewState = (PumpState)-1);

  void startPump(bool ResetStatus = false);  ///< Turn the pump ON
  void stopPump(bool ResetStatus = false);  ///< Turn the pump OFF
  void disablePump();  ///< Disable the pump
  void startMixing(int TimeOutSec = 0);
  
  void turnBypassOn(bool UserRequest);
  void turnBypassOff(bool UserRequest);
    
  void startPrime();
  void startRunning();
  void startBlowOff();


  PumpState getState();
  bool getOnState();  ///< Pump ON(1)/OFF(0)
  bool getEnabledState();  ///< Pump ENABLED(1)/DISABLED(0) 
  char * getStateText();  ///< Text representation of the current state: ON/OFF/DISABLED
  //void checkRelay(); ///< Flit the relay into the correct status ON/OFF based on PumpOn variable
  


  
  void setTimeOut(int NewTime);
  int getTimeOut();
  //void setPrimeTime(int NewTime);
  //int setPrimeTime();
  //void setBlowOffTime(int NewTime);
  //int getBlowOffTime();
  uint32_t PumpTimer = millis();  ///< Used for Pump runtime tracking
  
private:
  
 
protected:
  Module *Parent;
  uint8_t *PumpPin = NULL;  ///< < Pump relay pin
  uint8_t *BypassSolenoidPin  = NULL; ///< < Bypass solenoid relay pin
  bool PumpOn = false;  ///< true turns the pump on
  bool BypassOn = false;  ///< true turns the bypass solenoid on
  bool *PumpEnabled;  ///< Enable/disable pump. false= Block running the pump
  int RunTime = 0;  ///< Max pump run time in seconds  
  int *PumpTimeOut = NULL;  ///< Max pump run time in seconds
  int *PrimeTime = NULL;    ///< < (Sec) For wow long to keep the bypass solenoid on when starting the pump - Remove air bubbles from pump intake side
  int *BlowOffTime = NULL;     ///< < (Sec) For how long to open the bypass solenoid on after turning the pump off - Release pressure from pump discharge side
      
};