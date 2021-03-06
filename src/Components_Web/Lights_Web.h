#pragma once

#include "420Common_Web.h"
#include "420Module_Web.h"
#include "../Components/Lights.h"

///Light controller with PWM dimming - website component

class GrowBox; ///forward declaration

class Lights_Web : public Lights, public Common_Web
{
public:
  Lights_Web(const __FlashStringHelper *Name, Module_Web *Parent, Settings::LightsSettings *DefaultSettings); ///constructor
  void websiteEvent_Load(__attribute__((unused)) char *url);
  void websiteEvent_Refresh(__attribute__((unused)) char *url);
  void websiteEvent_Button(char *Button);
  void websiteEvent_Field(char *Field);  

private:
  
protected:
  Module_Web *Parent;
};