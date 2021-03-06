#include "HempyModule_Web.h"

HempyModule_Web::HempyModule_Web(const __FlashStringHelper *Name, Module_Web *Parent,Settings::HempyModuleSettings *DefaultSettings) : Common(Name), Common_Web(Name)
{ ///Constructor
  this->Parent = Parent;
  this->DefaultSettings = DefaultSettings;
  updateCommand();
  memcpy_P(this->WirelessChannel,(PGM_P)Name,sizeof(this->WirelessChannel));
  Parent->addToReportQueue(this);          ///Subscribing to the report queue: Calls the report() method
  Parent->addToRefreshQueue_Sec(this);     ///Subscribing to the 1 sec refresh queue: Calls the refresh_Sec() method
  Parent->addToRefreshQueue_FiveSec(this);     ///Subscribing to the 5 sec refresh queue: Calls the refresh_FiveSec() method
  Parent->addToRefreshQueue_Minute(this);      ///Subscribing to the 1 minute refresh queue: Calls the refresh_Minute() method
  //addToRefreshQueue_QuarterHour(this); ///Subscribing to the 30 minutes refresh queue: Calls the refresh_QuarterHour() method
  Parent->addToWebsiteQueue_Load(this);        ///Subscribing to the Website load event
  Parent->addToWebsiteQueue_Refresh(this);     ///Subscribing to the Website refresh event
  Parent->addToWebsiteQueue_Field(this);       ///Subscribing to the Website field submit event
  Parent->addToWebsiteQueue_Button(this);      ///Subscribing to the Website button press event
  logToSerials(F("HempyModule_Web object created"), true, 1);
}

void HempyModule_Web::report()
{
  Common::report();
  memset(&LongMessage[0], 0, sizeof(LongMessage)); ///clear variable
  strcat_P(LongMessage, (PGM_P)F("Bucket1 {"));
  strcat_P(LongMessage, (PGM_P)F("Weight:"));
  strcat(LongMessage, weightToText(Response.Weight_B1));
  strcat_P(LongMessage, (PGM_P)F(" ["));
  strcat(LongMessage, toText(Command.StartWeightBucket_B1));
  strcat_P(LongMessage, (PGM_P)F("/"));
  strcat(LongMessage, toText(Command.StopWeightBucket_B1));
  strcat_P(LongMessage, (PGM_P)F("] ; Pump:"));
  strcat(LongMessage, pumpStateToText(Response.PumpEnabled_B1,Response.PumpOn_B1));
  strcat_P(LongMessage, (PGM_P)F("}, Bucket2 {"));
  strcat_P(LongMessage, (PGM_P)F("Weight:"));
  strcat(LongMessage, weightToText(Response.Weight_B2));
  strcat_P(LongMessage, (PGM_P)F(" ["));
  strcat(LongMessage, toText(Command.StartWeightBucket_B2));
  strcat_P(LongMessage, (PGM_P)F("/"));
  strcat(LongMessage, toText(Command.StopWeightBucket_B2));
  strcat_P(LongMessage, (PGM_P)F("] ; Pump:"));
  strcat(LongMessage, pumpStateToText(Response.PumpEnabled_B2,Response.PumpOn_B2));
  strcat_P(LongMessage, (PGM_P)F("}"));
  logToSerials(&LongMessage, true, 1);
}

void HempyModule_Web::websiteEvent_Load(char *url)
{
  if (strncmp(url, "/G",2) == 0)
  {
    WebServer.setArgFloat(getComponentName(F("B1Strt")), Command.StartWeightBucket_B1);
    WebServer.setArgFloat(getComponentName(F("B1Stp")), Command.StopWeightBucket_B1);
    WebServer.setArgInt(getComponentName(F("B1Time")), Command.TimeOutPump_B1);
    WebServer.setArgFloat(getComponentName(F("B2Strt")), Command.StartWeightBucket_B2);
    WebServer.setArgFloat(getComponentName(F("B2Stp")), Command.StopWeightBucket_B2);
    WebServer.setArgInt(getComponentName(F("B2Time")), Command.TimeOutPump_B2);
  }
}

void HempyModule_Web::websiteEvent_Refresh(__attribute__((unused)) char *url) ///called when website is refreshed.
{
  if (strncmp(url, "/G",2) == 0)
  {
    WebServer.setArgString(getComponentName(F("B1Weight")), weightToText(Response.Weight_B1));
    WebServer.setArgString(getComponentName(F("B2Weight")), weightToText(Response.Weight_B2));
    WebServer.setArgString(getComponentName(F("B1Pump")),  pumpStateToText(Response.PumpEnabled_B1,Response.PumpOn_B1));
    WebServer.setArgString(getComponentName(F("B2Pump")), pumpStateToText(Response.PumpEnabled_B2,Response.PumpOn_B2));
  }
}

void HempyModule_Web::websiteEvent_Button(char *Button)
{ ///When a button is pressed on the website
  if (!isThisMyComponent(Button))
  {
    return;
  }
  else
  {    
    if (strcmp_P(ShortMessage, (PGM_P)F("B1PumpOn")) == 0)
    {
      Command.TurnOnPump_B1 = true;
      Parent -> addToLog(F("Watering HempyBucket 1"), false);
    }
    else if (strcmp_P(ShortMessage, (PGM_P)F("B2PumpOn")) == 0)
    {
      Command.TurnOnPump_B2 = true;
      Parent -> addToLog(F("Watering HempyBucket 2"), false);
    } 
    else if (strcmp_P(ShortMessage, (PGM_P)F("B1PumpOff")) == 0)
    {
      Command.TurnOffPump_B1 = true;
      Parent -> addToLog(F("Stop watering HempyBucket 1"), false);
    }
    else if (strcmp_P(ShortMessage, (PGM_P)F("B2PumpOff")) == 0)
    {
      Command.TurnOffPump_B2 = true;
      Parent -> addToLog(F("Stop watering HempyBucket 2"), false);
    }
    else if (strcmp_P(ShortMessage, (PGM_P)F("B1PumpDis")) == 0)
    {
      Command.DisablePump_B1 = true;
      Parent -> addToLog(F("Disabled HempyBucket 1 pump"), false);
    } 
    else if (strcmp_P(ShortMessage, (PGM_P)F("B2PumpDis")) == 0)
    {
      Command.DisablePump_B2 = true;
      Parent -> addToLog(F("Disabled HempyBucket 2 pump"), false);
    }  
    SyncRequested = true;  
  }
}


void HempyModule_Web::websiteEvent_Field(char *Field)
{ ///When the website field is submitted
  if (!isThisMyComponent(Field))
  {
    return;
  }
  else
  {    
    if (strcmp_P(ShortMessage, (PGM_P)F("B1Strt")) == 0)
    {
      DefaultSettings->StartWeightBucket_B1 = WebServer.getArgFloat();      
    }  /// \todo Write back to EEPROM
    else if (strcmp_P(ShortMessage, (PGM_P)F("B1Stp")) == 0)
    {
      DefaultSettings->StopWeightBucket_B1 = WebServer.getArgFloat();
      Parent -> addToLog(F("Bucket 1 limits updated"), false);
    }
    else if (strcmp_P(ShortMessage, (PGM_P)F("B1Time")) == 0)
    {
      DefaultSettings->TimeOutPump_B1 = WebServer.getArgInt();
      Parent -> addToLog(F("Pump 1 timeout updated"), false);
    }
    else if (strcmp_P(ShortMessage, (PGM_P)F("B2Strt")) == 0)
    {
      DefaultSettings->StartWeightBucket_B2 = WebServer.getArgFloat();
    }
    else if (strcmp_P(ShortMessage, (PGM_P)F("B2Stp")) == 0)
    {
      DefaultSettings->StopWeightBucket_B2 = WebServer.getArgFloat();
      Parent -> addToLog(F("Bucket 2 limits updated"), false);
    }
    else if (strcmp_P(ShortMessage, (PGM_P)F("B2Time")) == 0)
    {
      DefaultSettings->TimeOutPump_B2 = WebServer.getArgInt();
      Parent -> addToLog(F("Pump 2 timeout updated"), false);
    }
    SyncRequested = true;
  }
}

void HempyModule_Web::refresh_Sec()
{
  if (*Debug)
    Common::refresh_Sec();  
  if(SyncRequested) {
    SyncRequested = false;
    syncModule(WirelessChannel,&Command,&Response);    
  }
}

void HempyModule_Web::refresh_FiveSec()
{
  if (*Debug)
    Common::refresh_FiveSec();  
  syncModule(WirelessChannel,&Command,&Response);
}

void HempyModule_Web::refresh_Minute()
{
  if (*Debug)
    Common::refresh_Minute();  
}

void HempyModule_Web::syncModule( const byte WirelessChannel[], hempyCommand *Command, hempyResponse *Response){
  updateCommand();
  logToSerials(F("Sending sync command..."),false,3);
  Parent -> Wireless -> openWritingPipe(WirelessChannel);
  if (Parent -> Wireless -> write(Command, sizeof(*Command) )) {
      if ( Parent -> Wireless -> isAckPayloadAvailable() ) {
          Parent -> Wireless -> read(Response, sizeof(*Response));
           logToSerials(F("Acknowledgement received ["),false,2);            
          Serial.print(sizeof(*Response)); /// \todo Use LogToSerial
          logToSerials(F("bytes]"),true,1);

          if(*Debug){
          logToSerials(Response -> PumpOn_B1,false,3);
          logToSerials(F(","),false,1);
          logToSerials(Response -> PumpEnabled_B1,false,1);
          logToSerials(F(","),false,1);
          logToSerials(Response -> Weight_B1,false,1);
          logToSerials(F(","),false,1);
          logToSerials(Response -> PumpOn_B2,false,1);
          logToSerials(F(","),false,1);
          logToSerials(Response -> PumpEnabled_B2,false,1);
          logToSerials(F(";"),false,1);
          logToSerials(Response -> Weight_B2,true,1);               
          }            

          ///Turn off command flags 
          if(Command -> DisablePump_B1 || Command -> TurnOnPump_B1 || Command -> TurnOffPump_B1 || Command -> DisablePump_B2 || Command -> TurnOnPump_B2 || Command -> TurnOffPump_B2 )
          {
            SyncRequested = true;  ///Force a second packet to actualize the response
            Command -> DisablePump_B1 = false;
            Command -> TurnOnPump_B1 = false;
            Command -> TurnOffPump_B1 = false;
            Command -> DisablePump_B2 = false;
            Command -> TurnOnPump_B2 = false;
            Command -> TurnOffPump_B2 = false;
          }
      }
      else {
          Serial.println(F(" Acknowledgement received without any data."));
      }        
  }
  else {
      Serial.println(F(" No response."));
  }
  }


void HempyModule_Web::updateCommand() {        // so you can see that new data is being sent
    //Command.TurnOnPump_B1 = random(0,2);  //Generate random bool: 0 or 1. The max limit is exclusive!
    //Command.TurnOnPump_B2 = random(0,2);    
    Command.Time = now();
    Command.Debug = *Debug;
    Command.Metric = *Metric;
    Command.StartWeightBucket_B1= DefaultSettings->StartWeightBucket_B1;
    Command.StopWeightBucket_B1= DefaultSettings->StopWeightBucket_B1;
    Command.TimeOutPump_B1= DefaultSettings->TimeOutPump_B1;
    Command.StartWeightBucket_B2= DefaultSettings->StartWeightBucket_B2;
    Command.StopWeightBucket_B2= DefaultSettings->StopWeightBucket_B2;
    Command.TimeOutPump_B2= DefaultSettings->TimeOutPump_B2;
}