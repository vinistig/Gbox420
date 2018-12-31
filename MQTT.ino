//Global variables
char MqttPath[64];  //buffer

//Global constants
const char* MqttROOT = "growboxguy@gmail.com/";
const char* MqttPUBLISH = "Gbox420";
const char* MqttLwtTopic = "LWT";  //When the connection is lost the MQTT broker will publish a final message to this topic
const char* MqttLwtMessage = "Gbox420 Offline"; //this is the message subscribers will get under the topic specified by MqttLwtTopic variable when the box goes offline
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
const char* MqttPumpDisable = "PumpDisable";
const char* MqttPressureCalibrate = "PressureCalibrate";
const char* MqttNtpTime = "NtpTime";
//const char* Mqtt = "";

void setupMqtt(){
  Mqtt.connectedCb.attach(mqttConnected);
  Mqtt.disconnectedCb.attach(mqttDisconnected);
  Mqtt.publishedCb.attach(mqttPublished);
  Mqtt.dataCb.attach(mqttReceived);
  memset(&MqttPath[0], 0, sizeof(MqttPath)); //reset variable to store the Publish to path
  strcat(MqttPath,MqttROOT);
  strcat(MqttPath,MqttLwtTopic);
  Mqtt.lwt(MqttPath, MqttLwtMessage, 0, 1); //(topic,message,qos,retain) declares what message should be sent on it's behalf by the broker after Gbox420 has gone offline.
  Mqtt.setup();
}

void mqttReceived(void* response) {
  ELClientResponse *res = (ELClientResponse *)response;
  char topic[64];
  char data[16]; 
  ((*res).popString()).toCharArray(topic, 64);
  ((*res).popString()).toCharArray(data, 16);

  LogToSerials(F("Received: "),false);LogToSerials(topic,false);LogToSerials(F(" - "),false);LogToSerials(data,true);
  if(strstr(topic,MqttLights)!=NULL) { if(strcmp(data,"1")==0)turnLightON(true); else if(strcmp(data,"0")==0)turnLightOFF(true); }
  else if(strstr(topic,MqttBrightness)!=NULL) { setBrightness(atoi(data),true); }
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
  else if(strstr(topic,MqttPumpDisable)!=NULL) { aeroPumpDisable();}  
  else if(strstr(topic,MqttPressureCalibrate)!=NULL) { calibratePressureSensor();}
  else if(strstr(topic,MqttNtpTime)!=NULL) { UpdateNtpTime = true;}
}

void mqttPublush(bool AddToLog){ //publish readings in JSON format
  if(AddToLog)addToLog(F("Reporting to MQTT"));
  memset(&WebMessage[0], 0, sizeof(WebMessage));  //clear variable
  strcat_P(WebMessage,(PGM_P)F("{\"BoxDate\":\""));  strcat(WebMessage,CurrentTime);
  strcat_P(WebMessage,(PGM_P)F("\",\"BoxTempC\":\""));  strcat(WebMessage,toText(BoxTempC));
  strcat_P(WebMessage,(PGM_P)F("\",\"BoxTempF\":\""));  strcat(WebMessage,toText(BoxTempF));
  strcat_P(WebMessage,(PGM_P)F("\",\"Humidity\":\""));  strcat(WebMessage,toText(Humidity));
  strcat_P(WebMessage,(PGM_P)F("\",\"Power\":\""));  strcat(WebMessage,toText(Power)); 
  strcat_P(WebMessage,(PGM_P)F("\",\"Energy\":\""));  strcat(WebMessage,toText(Energy));
  strcat_P(WebMessage,(PGM_P)F("\",\"Voltage\":\""));  strcat(WebMessage,toText(Voltage));
  strcat_P(WebMessage,(PGM_P)F("\",\"Current\":\""));  strcat(WebMessage,toText(Current));
  strcat_P(WebMessage,(PGM_P)F("\",\"PH\":\""));  strcat(WebMessage,toText(PH));
  strcat_P(WebMessage,(PGM_P)F("\",\"Moisture\":\""));  strcat(WebMessage,toText(Moisture));
  strcat_P(WebMessage,(PGM_P)F("\",\"isLightOn\":\""));  strcat(WebMessage,toText(MySettings.isLightOn));
  strcat_P(WebMessage,(PGM_P)F("\",\"Brightness\":\""));  strcat(WebMessage,toText(MySettings.LightBrightness));
  strcat_P(WebMessage,(PGM_P)F("\",\"LightReading\":\""));  strcat(WebMessage,toText(LightReading));
  strcat_P(WebMessage,(PGM_P)F("\",\"isBright\":\""));  strcat(WebMessage,toText(isBright));
  strcat_P(WebMessage,(PGM_P)F("\",\"Reservoir\":\""));  strcat(WebMessage,toText(reservoirPercent));
  strcat_P(WebMessage,(PGM_P)F("\",\"InternalFan\":\"")); strcat_P(WebMessage,(PGM_P)internalFanSpeedToText());
  strcat_P(WebMessage,(PGM_P)F("\",\"ExhaustFan\":\"")); strcat_P(WebMessage,(PGM_P)exhaustFanSpeedToText()); 
  strcat_P(WebMessage,(PGM_P)F("\"}"));
  memset(&MqttPath[0], 0, sizeof(MqttPath)); //reset variable
  strcat(MqttPath,MqttROOT);
  strcat(MqttPath,MqttPUBLISH);
  LogToSerials(F("Reporting to MQTT: "),false);LogToSerials(MqttPath,false); LogToSerials(F(" - "),false); LogToSerials(WebMessage,true);
  Mqtt.publish(MqttPath, WebMessage,0,1); //(topic,message,qos,retain)
}

void mqttConnected(void* response) {
  memset(&MqttPath[0], 0, sizeof(MqttPath)); //reset variable
  strcat(MqttPath,MqttROOT);
  strcat_P(MqttPath,(PGM_P)F("#"));
  Mqtt.subscribe(MqttPath);
  LogToSerials(F("MQTT connected!"),true);
}

void mqttDisconnected(void* response) {
  LogToSerials(F("MQTT disconnected"),true);
}

void mqttPublished(void* response) {
  LogToSerials(F("MQTT published"),true);
}
