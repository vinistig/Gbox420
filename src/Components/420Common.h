#pragma once ///header guard, same as the ifndef/define guard  https:///en.wikipedia.org/wiki/Pragma_once

///Parent of all components

#include "Arduino.h"     
#include "TimeLib.h"     ///keeping track of time
#include "../../Settings.h" ///for loading defaults and storing/reading user settings
#include "../Helpers.h"  ///global functions
#include "../../SerialLog.h"  ///global functions


///forward declaration
extern char LongMessage[MaxLongTextLength];
extern char ShortMessage[MaxShotTextLength];
extern char CurrentTime[MaxTextLength];
extern Settings * ModuleSettings;
extern bool *Debug;
extern bool *Metric;

///This is a virtual class from which all other classes inherit from
///Enforces a set of Common functions shared by the components
class Common
{
public:
  const __FlashStringHelper *Name;
  bool isThisMyComponent(char const *lookupName); ///When it return true the component belongs to this object
  char *getComponentName(const __FlashStringHelper *Name);
  virtual void report();
  virtual void refresh_Sec();
  virtual void refresh_FiveSec();
  virtual void refresh_Minute();
  virtual void refresh_QuarterHour();
  /*
  void websiteEvent_Load(__attribute__((unused)) char *url){};
  void websiteEvent_Refresh(__attribute__((unused)) char *url){};
  void websiteEvent_Button(__attribute__((unused)) char *Button){};
  void websiteEvent_Field(__attribute__((unused)) char *Field){};
  */
   
private:
protected:
  Common(const __FlashStringHelper *Name); ///Constructor
  Common(){};
};
