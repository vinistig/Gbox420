#pragma once

#include "420Common.h"
#include "Aeroponics.h"

///Aeroponics tote without pressure tank

class Aeroponics_NoTank : public Aeroponics
{
public:
  Aeroponics_NoTank(const __FlashStringHelper *Name, Module *Parent, Settings::AeroponicsSettings *DefaultSettings, PressureSensor *FeedbackPressureSensor, WaterPump *Pump); ///constructor
  void refresh_Sec();
  void report();
  void sprayNow(bool UserRequest = false);
  void sprayOff(bool UserRequest = false);
  float LastSprayPressure = 0; ///tracks the last average pressure during a spray cycle

private:
 
protected:
  char *sprayStateToText();
  int *BlowOffTime;               ///After spraying open the bypass valve for X seconds to release pressure in the system
  bool BlowOffInProgress = false; ///Aeroponics - True while bypass valve is open during a pressure blow-off. Only used without the Pressure Tank option.
};