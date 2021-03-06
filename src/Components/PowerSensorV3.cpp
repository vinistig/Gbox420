#include "PowerSensorV3.h"

PowerSensorV3::PowerSensorV3(const __FlashStringHelper *Name, Module *Parent, HardwareSerial *SerialPort) : Common(Name)
{
  this->Parent = Parent;
  Sensor = new PZEM004Tv30(SerialPort);
  Parent->addToReportQueue(this);          ///Subscribing to the report queue: Calls the report() method
  Parent->addToRefreshQueue_FiveSec(this); ///Subscribing to the 5 sec refresh queue: Calls the refresh_FiveSec() method
  logToSerials(F("PowerSensorV3 object created"), true, 1);
}

void PowerSensorV3::refresh_FiveSec()
{
  if (*Debug)
    Common::refresh_Minute();
  Voltage = Sensor->voltage();      ///AC Voltage (V)
  Current = Sensor->current();      ///Current (A)
  Power = Sensor->power();          ///Actual power usage (W)
  Energy = Sensor->energy() / 1000; ///total power consumption (kWh)
  Frequency = Sensor->frequency();  ///total power consumption (kWh)
  PowerFactor = Sensor->pf();       ///total power consumption (kWh)
}

void PowerSensorV3::report()
{
  Common::report();
  memset(&LongMessage[0], 0, sizeof(LongMessage)); ///clear variable
  strcat_P(LongMessage, (PGM_P)F("Power:"));
  strcat(LongMessage, getPowerText(true));
  strcat_P(LongMessage, (PGM_P)F(" ; Total:"));
  strcat(LongMessage, getEnergyText(true));
  strcat_P(LongMessage, (PGM_P)F(" ; Voltage:"));
  strcat(LongMessage, getVoltageText(true));
  strcat_P(LongMessage, (PGM_P)F(" ; Current:"));
  strcat(LongMessage, getCurrentText(true));
  strcat_P(LongMessage, (PGM_P)F(" ; Frequency:"));
  strcat(LongMessage, getFrequencyText(true));
  strcat_P(LongMessage, (PGM_P)F(" ; PowerFactor:"));
  strcat(LongMessage, getPowerFactorText());
  logToSerials(&LongMessage, true, 1);
}

char *PowerSensorV3::getPowerText(bool IncludeUnits)
{
  if (IncludeUnits)
  {
    static char ReturnChar[MaxTextLength] = "";    ///each call will overwrite the same variable
    memset(&ReturnChar[0], 0, sizeof(ReturnChar)); ///clear variable
    strcat(ReturnChar, toText(Power));
    if (IncludeUnits)
      strcat_P(ReturnChar, (PGM_P)F("W"));
    return ReturnChar;
  }
  else
    return toText(Power);
}

char *PowerSensorV3::getEnergyText(bool IncludeUnits)
{
  if (IncludeUnits)
  {
    static char ReturnChar[MaxTextLength] = "";    ///each call will overwrite the same variable
    memset(&ReturnChar[0], 0, sizeof(ReturnChar)); ///clear variable
    strcat(ReturnChar, toText(Energy));
    if (IncludeUnits)
      strcat_P(ReturnChar, (PGM_P)F("kWh"));
    return ReturnChar;
  }
  else
    return toText(Energy);
}

char *PowerSensorV3::getVoltageText(bool IncludeUnits)
{
  if (IncludeUnits)
  {
    static char ReturnChar[MaxTextLength] = "";    ///each call will overwrite the same variable
    memset(&ReturnChar[0], 0, sizeof(ReturnChar)); ///clear variable
    strcat(ReturnChar, toText(Voltage));
    if (IncludeUnits)
      strcat_P(ReturnChar, (PGM_P)F("V"));
    return ReturnChar;
  }
  else
    return toText(Voltage);
}

char *PowerSensorV3::getCurrentText(bool IncludeUnits)
{
  if (IncludeUnits)
  {
    static char ReturnChar[MaxTextLength] = "";    ///each call will overwrite the same variable
    memset(&ReturnChar[0], 0, sizeof(ReturnChar)); ///clear variable
    strcat(ReturnChar, toText(Current));
    if (IncludeUnits)
      strcat_P(ReturnChar, (PGM_P)F("A"));
    return ReturnChar;
  }
  else
    return toText(Current);
}

char *PowerSensorV3::getFrequencyText(bool IncludeUnits)
{
  if (IncludeUnits)
  {
    static char ReturnChar[MaxTextLength] = "";    ///each call will overwrite the same variable
    memset(&ReturnChar[0], 0, sizeof(ReturnChar)); ///clear variable
    strcat(ReturnChar, toText(Frequency));
    strcat_P(ReturnChar, (PGM_P)F("Hz"));
    return ReturnChar;
  }
  else
    return toText(Frequency);
}

char *PowerSensorV3::getPowerFactorText()
{
  return toText(PowerFactor);
}