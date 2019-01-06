void readDHTSensor(){
  Humidity = TempSensor.readHumidity();
  BoxTempC= TempSensor.readTemperature();
  BoxTempF = BoxTempC *1.8 + 32;
  checkDHTAlerts();
  checkFanAutomation();
}

void checkDHTAlerts(){
  if(!VentOK && AlertLowTemp < BoxTempC  && BoxTempC < AlertHighTemp && AlertLowHumidity < Humidity  && Humidity < AlertHighHumidity){ //alert was active, but readings are back to normal
    sendEmailAlert(F("Ventillation%20OK")); 
    VentOK = true; //everything OK
  }
  if(VentOK && (isnan(BoxTempC) || BoxTempC < AlertLowTemp ||  AlertHighTemp < BoxTempC)){ //if temp out of limits, isnan-Returns true if float is Not A Numeber: DHT library returns NAN if the value cannot be read
    sendEmailAlert(F("Temperature%20out%20of%20range")); 
    VentOK = false;
    addToLog(F("Temperature alert triggered"));
  }
  if(VentOK && (isnan(Humidity) || Humidity < AlertLowHumidity ||  AlertHighHumidity < Humidity)){ //if temp out of limits
    sendEmailAlert(F("Humidity%20out%20of%20range")); 
    VentOK = false;
    addToLog(F("Humidity alert triggered"));
  }
}

void checkFanAutomation(){
    if(MySettings.automaticInternalFan && !MySettings.isInternalFanHigh && BoxTempC > MySettings.internalFanSwitchTemp){ //if temp is above the limit turn the fan to High
    internalFanHigh();
    addToLog(F("InternalFan auto high"));
  }
  if(MySettings.automaticInternalFan && MySettings.isInternalFanHigh && BoxTempC < (MySettings.internalFanSwitchTemp-3)){ //backward switch only if temp is 3 degrees below limit: avoids constant switching
    internalFanLow();
    addToLog(F("InternalFan auto low"));
  }
  if(MySettings.automaticExhaustFan && MySettings.isExhaustFanOn && Humidity < MySettings.exhaustFanOffHumid){ //Humidity below Off limit: turn exhaust off
    exhaustFanOff();
    addToLog(F("ExhaustFan auto off"));
  }
  if(MySettings.automaticExhaustFan && !MySettings.isExhaustFanHigh && MySettings.exhaustFanHighHumid < Humidity){ //Humidity above High limit: set exhaust High
    exhaustFanHigh();
    addToLog(F("ExhaustFan auto high"));
  }
  if(MySettings.automaticExhaustFan && ((!MySettings.isExhaustFanOn && MySettings.exhaustFanLowHumid < Humidity) || (MySettings.isExhaustFanHigh && Humidity < MySettings.exhaustFanLowHumid))){ //if exhaust was OFF and humidity gets above LOW limit, or if exhaust was High and humidity gets below LOW limit: Turn the fan LOW
    exhaustFanLow();
    addToLog(F("ExhaustFan auto low"));
  }
}

void internalFanLow(){
  MySettings.isInternalFanHigh = false;
  MySettings.isInternalFanOn = true;
  addToLog(F("Internal fan Low")); 
  PlayOnSound=true;
}

void internalFanHigh(){
  MySettings.isInternalFanHigh = true;
  MySettings.isInternalFanOn = true;
  addToLog(F("Internal fan High"));
  PlayOnSound=true;
}

void internalFanOff(){
   MySettings.isInternalFanOn = false;
   MySettings.isInternalFanHigh = false;
   addToLog(F("Internal fan OFF"));
   PlayOffSound=true;
}

void exhaustFanLow(){
  MySettings.isExhaustFanHigh = false;
  MySettings.isExhaustFanOn = true;
  addToLog(F("Exhaust fan Low"));
  PlayOnSound=true;
}

void exhaustFanHigh(){
  MySettings.isExhaustFanHigh = true;
  MySettings.isExhaustFanOn = true;
  addToLog(F("Exhaust fan High"));
  PlayOnSound=true;
}

void exhaustFanOff(){
  MySettings.isExhaustFanOn = false;
  MySettings.isExhaustFanHigh = false;
  addToLog(F("Exhaust fan OFF"));
  PlayOffSound=true;
}

void autoFanOnOff(bool Status,bool Internal){
  if(Internal){
    MySettings.automaticInternalFan = Status;
    if(MySettings.automaticInternalFan){
      addToLog(F("Internal fan automatic"));
      PlayOnSound=true;
      }
    else{
      addToLog(F("Internal fan manual"));
      PlayOffSound=true; 
      }
  }
  else{
    MySettings.automaticExhaustFan = Status;
    if(MySettings.automaticExhaustFan){
      addToLog(F("Exhaust fan automatic"));
      PlayOnSound=true;
      }
    else{
      addToLog(F("Exhaust fan manual"));
      PlayOffSound=true; 
      }
  }
}

void setInternalSwitchTemp(int Temp){
  MySettings.internalFanSwitchTemp = Temp;
}

void setExhaustOffHumidity(int Humidity){
  MySettings.exhaustFanOffHumid = Humidity;
}

void setExhaustLowHumidity(int Humidity){
  MySettings.exhaustFanLowHumid = Humidity;
}

void setExhaustHighHumidity(int Humidity){
  MySettings.exhaustFanHighHumid = Humidity;
  addToLog(F("ExhaustFan limits updated"));
}

const __FlashStringHelper * fanSpeedToText(bool Internal){
  if(Internal){
   if(!MySettings.isInternalFanOn) return F("OFF");
   else if (MySettings.isInternalFanHigh) return F("HIGH");
   else return F("LOW");
  }
  else{
   if(!MySettings.isExhaustFanOn) return F("OFF");
   else if (MySettings.isExhaustFanHigh) return F("HIGH");
   else return F("LOW");
  }
}

const __FlashStringHelper * fanSpeedToNumber(bool internal){
   if(internal){
   if(!MySettings.isInternalFanOn) return F("0");
   else if (MySettings.isInternalFanHigh) return F("2");
   else return F("1");
  }
  else{
   if(!MySettings.isExhaustFanOn) return F("0");
   else if (MySettings.isExhaustFanHigh) return F("2");
   else return F("1");
  }
}
