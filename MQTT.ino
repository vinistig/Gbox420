//Global variables
bool MqttAlive = false;
long LastHeartBeat;
char MqttPath[64];

//Global constants
const char* MqttROOT = "/growboxguy@gmail.com/";
const char* MqttPUBLISH = "Gbox420";
const char* MqttLights = "Lights";  
const char* MqttBrightness = "Brightness";
const char* MqttDisplayBrightness = "DisplayBrightness";
const char* MqttTimerEnabled = "TimerEnabled";
const char* MqttSoundEnabled = "SoundEnabled";
const char* MqttLightsOnHour = "LightsOnHour";
const char* MqttLightsOnMinute = "LightsOnMinute";
const char* MqttLightsOffHour = "LightsOffHour";
const char* MqttLightsOffMinute = "LightsOffMinute";
const char* MqttAeroInterval = "AeroInterval";
const char* MqttAeroDuration = "AeroDuration";
const char* MqttAeroPressureLow = "AeroPressureLow";
const char* MqttAeroPressureHigh = "AeroPressureHigh";
const char* MqttAeroOffset = "AeroOffset";
const char* MqttSetTime = "SetTime";
const char* MqttAeroSprayEnabled = "AeroSprayEnabled";
const char* MqttLightCalibrate = "LightCalibrate";
const char* MqttPowersupply = "Powersupply";
const char* MqttEe = "Ee";
const char* MqttInternalFan = "InternalFan";
const char* MqttExhaustFan = "ExhaustFan";
const char* MqttGoogleSheets = "GoogleSheets";
const char* MqttSaveSettings = "SaveSettings";
const char* MqttAeroSprayNow = "AeroSprayNow";
const char* MqttAeroSprayOff = "AeroSprayOff";
const char* MqttPumpRefill = "PumpRefill";
const char* MqttPumpOff = "PumpOff";
const char* MqttPumpReset = "PumpReset";
const char* MqttPressureCalibrate = "PressureCalibrate";
const char* MqttNtpTime = "NtpTime";
//const char* Mqtt = "";

void mqttReceived(void* response) {
  ELClientResponse *res = (ELClientResponse *)response;
  char topic[64];
  char data[16]; 
  ((*res).popString()).toCharArray(topic, 64);
  ((*res).popString()).toCharArray(data, 16);

  Serial.print("Received: ");Serial.print(topic);Serial.print(" - ");Serial.println(data);
  if(strstr(topic,MqttPUBLISH)!=NULL) {MqttHeartBeat(); } //Subscribed to own MQTT feed: Confirming connection is still alive
  else if(strstr(topic,MqttLights)!=NULL) { if(strcmp(data,"1")==0)turnLightON(); else if(strcmp(data,"0")==0)turnLightOFF(); }
  else if(strstr(topic,MqttBrightness)!=NULL) { setBrightness(atoi(data)); }
  else if(strstr(topic,MqttDisplayBrightness)!=NULL) {setDigitDisplayBacklight(atoi(data));}
  else if(strstr(topic,MqttTimerEnabled)!=NULL) {setTimerOnOff(atoi(data));} //bool 
  else if(strstr(topic,MqttSoundEnabled)!=NULL) {setSoundOnOff(atoi(data));}
  else if(strstr(topic,MqttLightsOnHour)!=NULL) {setLightsOnHour(atoi(data));}
  else if(strstr(topic,MqttLightsOnMinute)!=NULL) {setLightsOnMinute(atoi(data));}
  else if(strstr(topic,MqttLightsOffHour)!=NULL) {setLightsOffHour(atoi(data));}
  else if(strstr(topic,MqttLightsOffMinute)!=NULL) {setLightsOffMinute(atoi(data));}       
  else if(strstr(topic,MqttAeroInterval)!=NULL) { setAeroInterval(atoi(data));}
  else if(strstr(topic,MqttAeroDuration)!=NULL) {setAeroDuration(atoi(data));}
  else if(strstr(topic,MqttAeroPressureLow)!=NULL) {setAeroPressureLow(atof(data));}
  else if(strstr(topic,MqttAeroPressureHigh)!=NULL) {setAeroPressureHigh(atof(data));} 
  else if(strstr(topic,MqttAeroOffset)!=NULL) {setAeroOffset(atof(data));} 
  else if(strstr(topic,MqttSetTime)!=NULL) {setTime(data);}
  else if(strstr(topic,MqttAeroSprayEnabled)!=NULL) {setAeroSprayOnOff(atoi(data));}
  else if(strstr(topic,MqttLightCalibrate)!=NULL) {triggerCalibrateLights();}
  else if(strstr(topic,MqttPowersupply)!=NULL) { if(strcmp(data,"1")==0)powerSupplyOn(); else if(strcmp(data,"0")==0)powerSupplyOff(); }
  else if(strstr(topic,MqttEe)!=NULL) { playEE(); }
  else if(strstr(topic,MqttInternalFan)!=NULL) {if(strcmp(data,"2")==0)internalFanHigh(); else if(strcmp(data,"1")==0)internalFanLow(); else if(strcmp(data,"0")==0)internalFanOff(); }
  else if(strstr(topic,MqttExhaustFan)!=NULL) {if(strcmp(data,"2")==0)exhaustFanHigh(); else if(strcmp(data,"1")==0)exhaustFanLow(); else if(strcmp(data,"0")==0)exhaustFanOff(); }
  else if(strstr(topic,MqttGoogleSheets)!=NULL) { ReportToGoogleSheets(true);} 
  else if(strstr(topic,MqttSaveSettings)!=NULL) { saveSettings(true);}
  else if(strstr(topic,MqttAeroSprayNow)!=NULL) { aeroSprayNow();}
  else if(strstr(topic,MqttAeroSprayOff)!=NULL) { aeroSprayOff();}  
  else if(strstr(topic,MqttPumpRefill)!=NULL) { aeroPumpRefill();}
  else if(strstr(topic,MqttPumpOff)!=NULL) { aeroPumpOff();}  
  else if(strstr(topic,MqttPumpReset)!=NULL) { aeroPumpReset();}
  else if(strstr(topic,MqttPressureCalibrate)!=NULL) { calibratePressureSensor();}
  else if(strstr(topic,MqttNtpTime)!=NULL) { UpdateNtpTime = true;}
}

void mqttPublush(bool LogMessage){ //publish readings in JSON format
  if(LogMessage)addToLog("Reporting to MQTT");
  memset(&WebMessage[0], 0, sizeof(WebMessage));  //clear variable
  strcat(WebMessage,"{\"BoxDate\":\"");  strcat(WebMessage,CurrentTime);
  strcat(WebMessage,"\",\"BoxTempC\":\"");  strcat(WebMessage,floatToChar(BoxTempC));
  strcat(WebMessage,"\",\"BoxTempF\":\"");  strcat(WebMessage,floatToChar(BoxTempF));
  strcat(WebMessage,"\",\"Humidity\":\"");  strcat(WebMessage,floatToChar(Humidity));
  strcat(WebMessage,"\",\"Power\":\"");  strcat(WebMessage,floatToChar(Power)); 
  strcat(WebMessage,"\",\"Energy\":\"");  strcat(WebMessage,floatToChar(Energy));
  strcat(WebMessage,"\",\"Voltage\":\"");  strcat(WebMessage,floatToChar(Voltage));
  strcat(WebMessage,"\",\"Current\":\"");  strcat(WebMessage,floatToChar(Current));
  strcat(WebMessage,"\",\"PH\":\"");  strcat(WebMessage,floatToChar(PH));
  strcat(WebMessage,"\",\"Moisture\":\"");  strcat(WebMessage,floatToChar(Moisture));
  strcat(WebMessage,"\",\"isLightOn\":\"");  strcat(WebMessage,intToChar(MySettings.isLightOn));
  strcat(WebMessage,"\",\"Brightness\":\"");  strcat(WebMessage,intToChar(MySettings.LightBrightness));
  strcat(WebMessage,"\",\"LightReading\":\"");  strcat(WebMessage,intToChar(LightReading));
  strcat(WebMessage,"\",\"isBright\":\"");  strcat(WebMessage,intToChar(isBright));
  strcat(WebMessage,"\",\"Reservoir\":\"");  strcat(WebMessage,intToChar(reservoirToPercent()));
  strcat(WebMessage,"\",\"InternalFan\":\""); strcat(WebMessage,fanSpeedToText(true));
  strcat(WebMessage,"\",\"ExhaustFan\":\""); strcat(WebMessage,fanSpeedToText(false)); 
  strcat(WebMessage,"\"}");
  memset(&MqttPath[0], 0, sizeof(MqttPath)); //reset variable
  strcat(MqttPath,MqttROOT);
  strcat(MqttPath,MqttPUBLISH);
  Serial.print("Reporting to MQTT: ");Serial.print(MqttPath); Serial.print(" - "); Serial.println(WebMessage);
  Mqtt.publish(MqttPath, WebMessage);

  if(millis() - LastHeartBeat > 1800000) //180sec - 3 reporting cycles, adjust this based on MQTT reporting frequency
    {
    bool MqttAlive = false;
    addToLog("MQTT Heartbeat stopped"); 
    }
}

void mqttConnected(void* response) {
  MqttAlive = true;
  memset(&MqttPath[0], 0, sizeof(MqttPath)); //reset variable
  strcat(MqttPath,MqttROOT);
  strcat(MqttPath,"#");
  Mqtt.subscribe(MqttPath);
  Serial.println("MQTT connected!");
}

void mqttDisconnected(void* response) {
  MqttAlive = false;  
  Serial.println("MQTT disconnected");
}

void mqttPublished(void* response) {
  Serial.println("MQTT published");
}

void setupMqtt(){
  Mqtt.connectedCb.attach(mqttConnected);
  Mqtt.disconnectedCb.attach(mqttDisconnected);
  Mqtt.publishedCb.attach(mqttPublished);
  Mqtt.dataCb.attach(mqttReceived);
  Mqtt.lwt("/lwt", "offline", 0, 0); //declares what message should be sent on it's behalf by the broker, after Gbox420 has gone offline.
  Mqtt.setup();
}

void MqttHeartBeat(){
  MqttAlive = true;
  LastHeartBeat = millis();
}
