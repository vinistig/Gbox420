#include "PressureSensor_Web.h"

PressureSensor_Web::PressureSensor_Web(const __FlashStringHelper *Name, Module *Parent, Settings::PressureSensorSettings *DefaultSettings) : PressureSensor(Name,Parent,DefaultSettings)
{
  this->Parent = Parent;  
  this->Parent->addToReportQueue(this);         //Subscribing to the report queue: Calls the report() method
  this->Parent->addToRefreshQueue_Minute(this); //Subscribing to the 1 minute refresh queue: Calls the refresh_Minute() method
  this->Parent->AddToWebsiteQueue_Load(this);   //Subscribing to the Website load event: Calls the websiteEvent_Load() method
  this->Parent->AddToWebsiteQueue_Button(this); //Subscribing to the Website button press event: Calls the websiteEvent_Button() method
  this->Parent->AddToWebsiteQueue_Field(this);  //Subscribing to the Website field submit event: Calls the websiteEvent_Field() method  
}

void PressureSensor_Web::websiteEvent_Load(__attribute__((unused)) char *url)
{
  if (strcmp(url, "/Settings.html.json") == 0)
  {
    WebServer.setArgString(getComponentName(F("Offset")), toPrecisionText(*Offset));
    WebServer.setArgString(getComponentName(F("Ratio")), toPrecisionText(*Ratio));
  }
}

void PressureSensor_Web::websiteEvent_Button(char *Button)
{
  if (!isThisMyComponent(Button))
  {
    return;
  }
  else
  {
    if (strcmp_P(ShortMessage, (PGM_P)F("ReadOffset")) == 0)
    {
      readOffset();
    }
  }
}

void PressureSensor_Web::websiteEvent_Field(char *Field)
{
  if (!isThisMyComponent(Field))
  {
    return;
  }
  else
  {
    if (strcmp_P(ShortMessage, (PGM_P)F("Offset")) == 0)
    {
      setOffset(WebServer.getArgFloat());
    }
    else if (strcmp_P(ShortMessage, (PGM_P)F("Ratio")) == 0)
    {
      setRatio(WebServer.getArgFloat());
    }
  }
}