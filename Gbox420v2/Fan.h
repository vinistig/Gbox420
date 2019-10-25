// strcat_P(LongMessage,(PGM_P)F(" ; Internal fan:"));strcat(LongMessage,fanSpeedToText(true));
// strcat_P(LongMessage,(PGM_P)F(" ; Exhaust fan:"));strcat(LongMessage,fanSpeedToText(false)); 

#pragma once

#include "420Common.h"

class GrowBox;  //forward declaration

class Fan : public Common
{
  public:
    Fan(const __FlashStringHelper * Name, GrowBox * GBox, Settings::FanSettings * DefaultSettings);
    GrowBox * GBox;

    void refresh();  //Called when component should refresh its state
    void report();
    void websiteLoadEvent(char * url){};
    void websiteRefreshEvent(char * url);
    void websiteButtonPressEvent(char * Button);
    void websiteFieldSubmitEvent(char * Field){};
    void checkFanStatus();
    void TurnOff();
    void SetLowSpeed();
    void SetHighSpeed();
    char * fanSpeedToText();
    char * fanSpeedToNumber();
  
  private:
    bool * State;
    bool * HighSpeed;     
    byte * OnOffPin;
    byte * SpeedPin;
};
