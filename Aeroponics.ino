void checkAero(bool Interrupt){
 if(!Interrupt)readAeroPressure(); //skip reading the pressure when called from an interrupt. (Within an Interroupt millis() counter doesn`t increase, so delay() never ends)
 if(MySettings.AeroPressureTankPresent)  {
    checkAeroSprayTimer_WithPressureTank();
    if(!Interrupt)checkAeroPumpAlerts_WithPressureTank();
  }
 else {
  checkAeroSprayTimer_WithoutPressureTank();
  if(!Interrupt)checkAeroPumpAlerts_WithoutPressureTank();
 }
 checkRelays();
}

void checkAeroSprayTimer_WithPressureTank(){ //when pressure tank is connected
  if(AeroPressure >= MySettings.AeroPressureHigh){ //If set high pressure is reached
      aeroPumpOff(false);
  }
 
 if(AeroSolenoidOn)    { //if spray is on   
    if (millis() - AeroPumpTimer >= MySettings.AeroPumpTimeout * 1000){ //have not reached high pressue within timeout limit
      aeroPumpDisable();
      sendEmailAlert(F("Aeroponics%20pump%20failed")); 
      addToLog(F("Pump failed"));    
    }
    if(millis() - AeroSprayTimer >= MySettings.AeroDuration * 1000){  //if time to stop spraying (AeroDuration in Seconds)
      AeroSolenoidOn = false;
      logToSerials(F("Stopping spray"),true);
      PlayOffSound = true;
      AeroSprayTimer = millis();
    }
  }
  else{ //if spray is off
    if(MySettings.AeroSprayEnabled && millis() - AeroSprayTimer >= MySettings.AeroInterval * 60000){ //if time to start spraying (AeroInterval in Minutes)
        AeroSolenoidOn = true;        
        logToSerials(F("Starting spray"),true);
        PlayOnSound = true;
        AeroSprayTimer = millis();
    }
    if( PumpOK && checkQuietTime() && AeroPressure <= MySettings.AeroPressureLow){ //if pump is not disabled and quiet time not active and Pressure reached low limit: turn on pump 
        aeroPumpOn(false);
    }   
  }  
}

void checkAeroSprayTimer_WithoutPressureTank(){ //pump directly connected to aeroponics tote, with an electronically controlled bypass valve
 if(AeroPumpOn)    { //if pump is on
    if(!AeroSolenoidOn && millis() - AeroSprayTimer >= MySettings.AeroDuration * 1000){  //bypass valve is closed and time to stop spraying (AeroDuration in Seconds)
      AeroPumpOn = false;      
      logToSerials(F("Stopping spray"),true);
      AeroSprayTimer = millis();
    }
    else{
      if (AeroSolenoidOn && millis() - AeroPumpTimer >= MySettings.AeroPrimingTime * 1000){ //self priming timeout reached, time to start spraying
        AeroSolenoidOn = false; //Close bypass valve
        stopFlowMeter();
        logToSerials(F("Closing bypass, starting spray"),true);
        AeroSprayTimer = millis();
        }
       }
     }
  else{ //pump is off
    AeroSolenoidOn = false; //Should not leave the solenoid turned on
    if(PumpOK && MySettings.AeroSprayEnabled  && millis() - AeroSprayTimer >= MySettings.AeroInterval * 60000){ //if time to start spraying (AeroInterval in Minutes)
      aeroPumpOn(false);
                     
    }    
  } 
}

void aeroSprayNow(bool DueToHighPressure){   
  if(MySettings.AeroSprayEnabled || DueToHighPressure){
    AeroSprayTimer = millis();
    AeroSolenoidOn = true;
    if(!MySettings.AeroPressureTankPresent){
        AeroPumpOn = true;
        startFlowMeter();
        AeroPumpTimer = millis();
       }    
    checkRelays();
    PlayOnSound = true;
    if(DueToHighPressure) addToLog(F("Above pressure limit,spraying"));
    else addToLog(F("Aeroponics spraying"));
    }
}

void aeroSprayOff(){   
    AeroSolenoidOn = false; 
    if(!MySettings.AeroPressureTankPresent) {
      AeroPumpOn = false; 
      stopFlowMeter(); 
    }
    checkRelays();
    addToLog(F("Aeroponics spray OFF"));
}

void aeroPumpOn(bool AddToLog){
  PumpOK = true;
  AeroPumpOn = true;
  AeroPumpTimer = millis();  
  if(!MySettings.AeroPressureTankPresent){ //Open bypass valve
      startFlowMeter();
      AeroSolenoidOn = true;
    } 
  checkRelays();
  PlayOnSound = true;
  if(AddToLog)addToLog(F("Pump ON")); 
}

void aeroPumpOff(bool AddToLog){  
  AeroPumpOn = false;
  if(!MySettings.AeroPressureTankPresent){ //Close bypass valve
      AeroSolenoidOn = false;
      stopFlowMeter();
   } 
  checkRelays();
  PlayOffSound = true;
  if(AddToLog)addToLog(F("Pump OFF"));
}

void aeroPumpDisable(){
  aeroPumpOff(false);
  PumpOK = false;
  addToLog(F("Pump disabled"));
}

void setAeroSprayOnOff(bool State){
  MySettings.AeroSprayEnabled=State;
  if(MySettings.AeroSprayEnabled){ 
    addToLog(F("Aeroponics spray enabled"));
    PlayOnSound=true;} 
  else {
    addToLog(F("Aeroponics spray disabled"));
    PlayOffSound=true;}
}

bool PrevoiusPressureRead = true;
void checkAeroPumpAlerts_WithPressureTank()
{
  if(MySettings.PressureAlertLow <= AeroPressure && AeroPressure <= MySettings.PressureAlertHigh){ //If pressure is between OK range
    if(PrevoiusPressureRead != true){PressureTriggerCount = 0;}
    else{ if(!PressureOK)PressureTriggerCount++; } 
    PrevoiusPressureRead = true;     
     
    if(!PressureOK && PressureTriggerCount>=MySettings.ReadCountBeforeAlert){ // pressure was not OK before
       PressureOK = true;
       sendEmailAlert(F("Aeroponics%20pressure%20OK"));
       } 
  }
  else{
    if(AeroPressure > MySettings.PressureAlertHigh){ //Pressure over limit - emergency spraying
          aeroPumpOff(false); //force pump off
          aeroSprayNow(true); //try to release pressure  
    }
    if(PrevoiusPressureRead != false){PressureTriggerCount = 0;}
    else{ if(PressureOK)PressureTriggerCount++; } 
    PrevoiusPressureRead = false;  
    
    if(PressureOK && PressureTriggerCount>=MySettings.ReadCountBeforeAlert){
        PressureOK = false;
        if(AeroPressure > MySettings.PressureAlertHigh){ //If high pressure alert level is reached   
          sendEmailAlert(F("Aeroponics%20pressure%20too%20high"));
          addToLog(F("High pressure warning"));
        }
        if(AeroPressure < MySettings.PressureAlertLow){ //If low pressure alert level is reached
          //if(PumpOK) aeroPumpOn(false); //Uncomment this to turn pump on even under quiet time
          sendEmailAlert(F("Aeroponics%20pressure%20too%20low"));
          addToLog(F("Low pressure warning"));
        } 
      }
   }     
}

void checkAeroPumpAlerts_WithoutPressureTank()
{
  if(AeroPumpOn){ //Only if pump is running
    if(!AeroSolenoidOn){ //when bypass is closed and should be spraying
      if( MySettings.PressureAlertLow <= AeroPressure && AeroPressure <= MySettings.PressureAlertHigh){ //If pressure is between OK range
        if(PrevoiusPressureRead != true){PressureTriggerCount = 0;}
        else{ if(!PressureOK)PressureTriggerCount++; } 
        PrevoiusPressureRead = true;     
         
        if(!PressureOK && PressureTriggerCount>=MySettings.ReadCountBeforeAlert){ // pressure was not OK before
           PressureOK = true;
           sendEmailAlert(F("Aeroponics%20pressure%20OK"));
           } 
       }
       else{ //if pressure is not OK
          if(AeroPressure > MySettings.PressureAlertHigh){ //Pressure over limit - emergency off
                aeroPumpOff(false); //force pump off
          }
          if(PrevoiusPressureRead != false){PressureTriggerCount = 0;}
          else{ if(PressureOK)PressureTriggerCount++; } 
          PrevoiusPressureRead = false;  
          
          if(PressureOK && PressureTriggerCount>=MySettings.ReadCountBeforeAlert){
              PressureOK = false;
              if(AeroPressure > MySettings.PressureAlertHigh){ //If high pressure alert level is reached   
                sendEmailAlert(F("Aeroponics%20pressure%20too%20high"));
                addToLog(F("High pressure warning"));
              }
              if(AeroPressure < MySettings.PressureAlertLow){ //If low pressure alert level is reached
                sendEmailAlert(F("Aeroponics%20pressure%20too%20low"));
                addToLog(F("Low pressure warning"));
              } 
            }
         }
    }     
  }
  else  {//if pump is off: should be no pressure
        if((MySettings.MetricSystemEnabled && AeroPressure <= 0.3 )  || (!MySettings.MetricSystemEnabled && AeroPressure <= 5)){ //Checking if readings are close to 0
          if(PrevoiusPressureRead != true){PressureTriggerCount = 0;}
          else{ if(!PressureOK)PressureTriggerCount++; } 
          PrevoiusPressureRead = true;     
           
          if(!PressureOK && PressureTriggerCount>=MySettings.ReadCountBeforeAlert){ // pressure was not OK before
             PressureOK = true;
             sendEmailAlert(F("Aeroponics%20pressure%20OK"));
             } 
         }
         else{ //if pressure is not OK
            if(PrevoiusPressureRead != false){PressureTriggerCount = 0;}
            else{ if(PressureOK)PressureTriggerCount++; } 
            PrevoiusPressureRead = false;  
            
            if(PressureOK && PressureTriggerCount>=MySettings.ReadCountBeforeAlert){
                PressureOK = false;
                  sendEmailAlert(F("Aeroponics%20sensor%20malfunction"));
                  addToLog(F("Pressure%20sensor%20malfunction"));
              }
           }     
  }
}

void readAeroPressure(){
  float  Reading=0;
  for(byte i=0;i<50;i++) { 
   Reading+=analogRead(PressureSensorInPin);
   delay(20);
  }
  Reading = Reading /50;
  float Voltage = ((float)Reading) * 5 / 1024 ;
  if(MySettings.MetricSystemEnabled) AeroPressure = MySettings.PressureSensorRatio*(Voltage-MySettings.PressureSensorOffset); // unit: bar / 100kPa
  else AeroPressure = MySettings.PressureSensorRatio*(Voltage-MySettings.PressureSensorOffset) * 14.5038;  //unit: PSI
}

void calibratePressureSensor(){  //Should only be called when there is 0 pressure
  float sum = 0;
  for(byte i = 0; i<50;i++){
  sum += analogRead(PressureSensorInPin) ; 
  delay(10);
  }  
  float AeroOffsetRecommendation = (sum/50)*5/1024; //Reads voltage at 0 pressure
  strncpy_P(LogMessage,(PGM_P)F("0 pressure AeroOffset: "),MaxTextLength);
  strcat(LogMessage,toText(AeroOffsetRecommendation));
  addToLog(LogMessage);
}

void setPressureSensorOffset(float Value){
  MySettings.PressureSensorOffset = Value;
  addToLog(F("Pressure offset updated"));  
}

void setPressureSensorRatio(float Value){
  MySettings.PressureSensorRatio = Value;
  addToLog(F("Pressure/voltage ratio updated"));  
}

void setAeroInterval(int interval){  
  MySettings.AeroInterval = interval; 
  AeroSprayTimer = millis();
}

void setAeroDuration(int duration){  
  MySettings.AeroDuration = duration;
  AeroSprayTimer = millis(); 
  addToLog(F("Spray time updated"));  
}


void setAeroPressureLow(float PressureLow){
  MySettings.AeroPressureLow =  PressureLow;
}

void setAeroPressureHigh(float PressureHigh){
  MySettings.AeroPressureHigh = PressureHigh;
  addToLog(F("Pump settings updated"));
}

const __FlashStringHelper * pumpStateToText(){
   if(!PumpOK) return F("DISABLED");
   else if(AeroPumpOn) return F("ON");
   else return F("OFF");
}

//Quiet time section: Blocks running the pump in a pre-defined time range
uint32_t AeroLastRefill= 0;
bool checkQuietTime() {  
  if(MySettings.AeroQuietEnabled){
    time_t Now = now(); // Get the current time
    int CombinedFromTime = MySettings.AeroQuietFromHour * 100 + MySettings.AeroQuietFromMinute;
    int CombinedToTime = MySettings.AeroQuietToHour * 100 + MySettings.AeroQuietToMinute;
    int CombinedCurrentTime = hour(Now) * 100 + minute(Now);
    if(MySettings.AeroRefillBeforeQuiet && PumpOK && CombinedFromTime == CombinedCurrentTime && millis() - AeroLastRefill > 120000 ){
      aeroPumpOn(false); 
      AeroLastRefill = millis();
    }
    if(CombinedFromTime <= CombinedToTime)  //no midnight turnover, Example: On 8:10, Off: 20:10
    {
      if(CombinedFromTime <= CombinedCurrentTime && CombinedCurrentTime < CombinedToTime){
        return false;} //do not allow running the pump
      else  return true;  //allow running   
    }
    else   //midnight turnover, Example: On 21:20, Off: 9:20
    {
      if(CombinedFromTime <= CombinedCurrentTime || CombinedCurrentTime < CombinedToTime){
       return false; } //do not allow running the pump
      else  return true;  //allow running    
    }
  }
  else return true; //always allow if quiet mode is not enabled
}

void setAeroPumpTimeout(int Timeout)
{
MySettings.AeroPumpTimeout = (uint32_t)Timeout;
addToLog(F("Aero pump timeout updated"));
}

void setAeroPrimingTime(int Timing)
{
MySettings.AeroPrimingTime = (uint32_t)Timing;
addToLog(F("Aero priming time updated"));
}

void setQuietOnOff(bool State){
  MySettings.AeroQuietEnabled = State;
  if(MySettings.AeroQuietEnabled){ 
    addToLog(F("Quiet mode enabled"));
    PlayOnSound=true;
    }
  else {
    addToLog(F("Quiet mode disabled"));
    PlayOffSound=true;
    }
}

void setQuietRefillOnOff(bool State){
  MySettings.AeroRefillBeforeQuiet = State;
  if(MySettings.AeroRefillBeforeQuiet){ 
    addToLog(F("Refill before quiet enabled"));
    PlayOnSound=true;
    }
  else {
    addToLog(F("Refill before quiet disabled"));
    PlayOffSound=true;
    }
}

void setAeroPressureTankOnOff(bool State){ //this change requires a different aeroponics setup
  MySettings.AeroPressureTankPresent = State;  
  if(MySettings.AeroPressureTankPresent){ 
    addToLog(F("!Pressure tank mode!"));
    PlayOnSound=true;
    }
  else {
    addToLog(F("!Direct pump mode!"));
    PlayOffSound=true;
    }
    aeroPumpDisable(); //for safety disable the pump 
}

void setQuietFromHour(int Hour){
  MySettings.AeroQuietFromHour = Hour; 
}

void setQuietFromMinute(int Minute){
  MySettings.AeroQuietFromMinute = Minute;
  addToLog(F("Pump quiet From time updated")); 
}

void setQuietToHour(int Hour){
  MySettings.AeroQuietToHour = Hour;
}

void setQuietToMinute(int Minute){
  MySettings.AeroQuietToMinute = Minute;
  addToLog(F("Pump quiet To time updated"));
}

//***Flow meter***
volatile unsigned int FlowPulseCount = 0; //volatile variables are stored in RAM: https://www.arduino.cc/reference/en/language/variables/variable-scope--qualifiers/volatile/
void startFlowMeter(){
  FlowPulseCount = 0;
  attachInterrupt(digitalPinToInterrupt(FlowMeterInPin), flowPulseCounter, FALLING);  //Mega2560 support interrupts on port 2, 3, 18, 19, 20, 21  
}

void stopFlowMeter(){
  detachInterrupt(digitalPinToInterrupt(FlowMeterInPin));  //Mega2560 support interrupts on port 2, 3, 18, 19, 20, 21  
    strcpy(LogMessage,toText((uint32_t)FlowPulseCount));
    strcat_P(LogMessage,(PGM_P)F(" pulses"));
    addToLog(LogMessage);
}

void flowPulseCounter(){
   ATOMIC_BLOCK(ATOMIC_RESTORESTATE) { FlowPulseCount++; }
}
