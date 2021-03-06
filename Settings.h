#pragma once

/*! 
 *  \brief     Use this to change the Default Settings for each component. Loaded when the Arduino starts, updated by user interaction on the website.
 *  \details   The Settings object is stored in EEPROM and kept between reboots. 
 *  \author    GrowBoxGuy
 *  \version   4.20
 *  \warning   Only use these in the setup() function, or when a user initiated change is stored. EEPROM has a write limit of 100.000 cycles
 *  \attention Never use the funtions defined here in loop() or a repeating cycle! EEPROM would wear out very fast
 *  \attention Update the Version number when you make change to the structure in the SAVED TO EEPROM secton. This will overwrite the EEPROM settings with the sketch defaults.
 */

static const uint8_t Version = 19; ///< Increment this when you make a change in the SAVED TO EEPROM secton

///THIS SECTION DOES NOT GET STORED IN EEPROM:
  ///Global constants
  static const uint8_t MaxTextLength = 32;      ///Default char * buffer for storing a word + null terminator. Memory intense!
  static const uint8_t MaxShotTextLength = 128; ///Default char * buffer for storing mutiple words. Memory intense!
  static const int MaxLongTextLength = 1024; ///Default char * buffer for storing a long text. Memory intense!
  static const uint8_t LogDepth = 5;                  ///Show X number of log entries on website. Be careful, Max 1024uint8_ts can be passed during a Website refresh event, incuding all parameters passed
  static const uint8_t QueueDepth = 32;               ///Limits the maximum number of active modules. Memory intense!
  static const uint8_t RollingAverageDepth = 10;               ///Limits the maximum number of active modules. Memory intense!

  ///Global variables
  extern char LongMessage[MaxLongTextLength];  ///temp storage for assembling long messages (REST API, MQTT API)
  extern char ShortMessage[MaxShotTextLength]; ///temp storage for assembling short messages (Log entries, Error messages)
  extern char CurrentTime[MaxTextLength];      ///buffer for storing current time in text

  ///nRF24L01+ wireless receiver
  static const uint8_t Wireless_CSNPin = 49;
  static const uint8_t Wireless_MISOPin = 50;
  static const uint8_t Wireless_MOSIPin = 51;
  static const uint8_t Wireless_SCKPin = 52;
  static const uint8_t Wireless_CEPin = 53;
  static const uint8_t Wireless_Delay = 6;  ///< How long to wait between each retry, in multiples of 250us. Max is 15. 0 means 250us, 15 means 4000us
  static const uint8_t Wireless_Retry = 5;  ///< How many retries before giving up, max 15

///SAVED TO EEPROM - Settings struct
  ///If you change things here, increase the Version variable in line 4
  typedef struct
  {
    bool Debug = true;          ///Logs debug messages to serial and web outputs
    bool Metric = true;   ///Switch between Imperial/Metric units. If changed update the default temp and pressure values too.
    char PushingBoxLogRelayID[MaxTextLength] = {"v755877CF53383E1"};   ///UPDATE THIS DeviceID of the PushingBox logging scenario 

    struct AeroModuleSettings
    {
      AeroModuleSettings(bool PressureTankPresent = false) : PressureTankPresent(PressureTankPresent) {}
      bool PressureTankPresent; 
    };
    struct AeroModuleSettings AeroModule1 = {.PressureTankPresent = false};
    
    struct AeroponicsSettings
    { ///Common settings for both inheriting classes: Aeroponics_Tank and Aeroponics_NoTank
      AeroponicsSettings(bool SprayEnabled = true, int Interval = 0, int Duration = 0, float HighPressure = 0.0) : SprayEnabled(SprayEnabled), Interval(Interval), Duration(Duration), HighPressure(HighPressure)   {}
      bool SprayEnabled; ///Enable/disable spraying cycle
      int Interval;        ///Spray every X minutes
      int Duration;        ///Spray time in seconds
      float HighPressure; ///Turn off pump above this pressure
    };
    struct AeroponicsSettings AeroT1_Common = {.SprayEnabled= true, .Interval=15, .Duration = 10, .HighPressure = 7.0};
    struct AeroponicsSettings AeroNT1_Common = {.SprayEnabled= true, .Interval=15, .Duration = 10, .HighPressure = 7.0};

    /*
    struct AeroponicsSettings_NoTankSpecific  ///<Settings for an Aeroponics setup WITHOUT a pressure tank
    {                           
      AeroponicsSettings_NoTankSpecific() : {}
    };
    struct AeroponicsSettings_NoTankSpecific AeroNT1_Specific = {};
    */

    struct AeroponicsSettings_TankSpecific
    { ///Settings for an Aeroponics setup WITH a pressure tank
      AeroponicsSettings_TankSpecific(uint8_t SpraySolenoidPin = 0, float LowPressure = 0.0) : SpraySolenoidPin(SpraySolenoidPin), LowPressure(LowPressure) {}
      uint8_t SpraySolenoidPin;    ///Spray solenoid relay pin
      float LowPressure;  ///Turn on pump below this pressure      
    };
    struct AeroponicsSettings_TankSpecific AeroT1_Specific = {.SpraySolenoidPin = 22, .LowPressure = 5.0};

    struct DHTSensorSettings
    { ///initialized via Designated initializer https:///riptutorial.com/c/example/18609/using-designated-initializers
      DHTSensorSettings(uint8_t Pin = 0, uint8_t Type = 0) : Pin(Pin), Type(Type) {}
      uint8_t Pin;
      uint8_t Type; ///Type defines the sensor type: 11 - DHT11, 12 - DHT12, 21 - DHT21 or AM2301 , 22 - DHT22
    };
    struct DHTSensorSettings IDHT = {.Pin = 43, .Type = 22};
    struct DHTSensorSettings EDHT = {.Pin = 44, .Type = 22};
    struct DHTSensorSettings DHT1 = {.Pin = 44, .Type = 22};

    struct FanSettings
    {
      FanSettings(uint8_t OnOffPin = 0, uint8_t SpeedPin = 0) : OnOffPin(OnOffPin), SpeedPin(SpeedPin) {}
      uint8_t OnOffPin;          ///Relay pin for power
      uint8_t SpeedPin;          ///Relay pin for speed selection
      bool State = true;      ///true - ON, false - OFF
      bool HighSpeed = false; ///true - High speed, false - Low speed
    };
    struct FanSettings IFan = {.OnOffPin = 25, .SpeedPin = 26};
    struct FanSettings EFan = {.OnOffPin = 27, .SpeedPin = 28};
      
    struct GrowModuleSettings{
      GrowModuleSettings(bool ReportToGoogleSheets, uint8_t SheetsReportingFrequency) : ReportToGoogleSheets(ReportToGoogleSheets) , SheetsReportingFrequency(SheetsReportingFrequency) {} 
      bool ReportToGoogleSheets;  ///Enable/disable reporting sensor readings to Google Sheets
      uint8_t SheetsReportingFrequency; ///How often to report to Google Sheets. Use 15 minute increments only! Min 15min, Max 1440 (1day)
      ///bool ReportToMqtt = true;    ///Controls reporting sensor readings to an MQTT broker
    };
    struct GrowModuleSettings Gbox1 = {.ReportToGoogleSheets = true, .SheetsReportingFrequency = 30};
    
    struct HempyBucketSettings  /// \todo This should be only needed in the Gbox420 Nano - Hempy Module
    {
      HempyBucketSettings( float StartWeight = 0.0, float StopWeight = 0.0) : StartWeight(StartWeight), StopWeight(StopWeight)  {}
      float StartWeight; ///Start watering below this weight
      float StopWeight;  ///Stop watering above this weight
    };
    struct HempyBucketSettings Bucket1 = { .StartWeight = 4.2, .StopWeight = 6.9};
    struct HempyBucketSettings Bucket2 = { .StartWeight = 4.2, .StopWeight = 6.9};

    struct HempyModuleSettings
    {
      HempyModuleSettings( float StartWeightBucket_B1 = 0.0, float StopWeightBucket_B1 = 0.0, int TimeOutPump_B1 = 0, float StartWeightBucket_B2 = 0.0, float StopWeightBucket_B2 = 0.0, int TimeOutPump_B2 = 0 ) : StartWeightBucket_B1(StartWeightBucket_B1), StopWeightBucket_B1(StopWeightBucket_B1), TimeOutPump_B1(TimeOutPump_B1), StartWeightBucket_B2(StartWeightBucket_B2), StopWeightBucket_B2(StopWeightBucket_B2), TimeOutPump_B2(TimeOutPump_B2) {}
      float StartWeightBucket_B1; ///Start watering below this weight
      float StopWeightBucket_B1;  ///Stop watering above this weight
      int TimeOutPump_B1;  ///Max pump runtime in seconds, target StopWeight should be reached before hitting this. Pump gets disabled if timeout is reached /// \todo Add email alert when pump fails
      float StartWeightBucket_B2; ///Start watering below this weight
      float StopWeightBucket_B2;  ///Stop watering above this weight   
      int TimeOutPump_B2;  
    };
    struct HempyModuleSettings HempyModule1 = {.StartWeightBucket_B1 = 4.2, .StopWeightBucket_B1 = 6.9, .TimeOutPump_B1 = 120, .StartWeightBucket_B2 = 4.2, .StopWeightBucket_B2 = 6.9, .TimeOutPump_B2 = 120};
    
    struct LightSensorSettings
    {
      LightSensorSettings(uint8_t DigitalPin = 0, uint8_t AnalogPin = 0) : DigitalPin(DigitalPin), AnalogPin(AnalogPin) {}
      uint8_t DigitalPin;
      uint8_t AnalogPin;
    };
    struct LightSensorSettings LtSen1 = {.DigitalPin = 42, .AnalogPin = A0};

    struct LightsSettings
    {
      LightsSettings(uint8_t RelayPin = 0, uint8_t DimmingPin = 0, uint8_t DimmingLimit = 0) : RelayPin(RelayPin), DimmingPin(DimmingPin), DimmingLimit(DimmingLimit) {} ///Designated initializer
      uint8_t RelayPin;                                                                                                                                            ///Power relay Port 8 - LED lights
      uint8_t DimmingPin;                                                                                                                                          ///PWM based dimming, connected to optocoupler`s base over 1k ohm resistor
      uint8_t DimmingLimit;                                                                                                                                        ///Sets the LED dimming limit (Usually around 5%)
      bool Status = true;                                                                                                                                       ///Startup status for lights: True-ON / False-OFF
      uint8_t Brightness = 15;                                                                                                                                     ///Light intensity: 0 - 100 range for controlling led driver output
      bool TimerEnabled = true;                                                                                                                                 ///Enable timer controlling lights
      uint8_t OnHour = 4;                                                                                                                                          ///Light ON time - hour
      uint8_t OnMinute = 20;                                                                                                                                       ///Light ON time - minute
      uint8_t OffHour = 16;                                                                                                                                        ///Light OFF time - hour
      uint8_t OffMinute = 20;                                                                                                                                      ///Light OFF time - minute
    };
    struct LightsSettings Lt1 = {.RelayPin = 29, .DimmingPin = 11, .DimmingLimit = 8}; ///Creating a LightSettings instance, passing in the unique parameters

    struct ModuleSkeletonSettings
    { ///Test module
      ModuleSkeletonSettings(bool PersistentBool = 0, float PersistentFloat = 0.0) : PersistentBool(PersistentBool), PersistentFloat(PersistentFloat) {}
      bool PersistentBool;
      int PersistentInt = 420; ///Same value on all instances
      float PersistentFloat;
    };
    struct ModuleSkeletonSettings ModuleSkeleton1 = {.PersistentBool = false, .PersistentFloat = 1.23}; ///Instance 1
    struct ModuleSkeletonSettings ModuleSkeleton2 = {.PersistentBool = true, .PersistentFloat = 4.56};  ///Instance 2

    struct PHSensorSettings
    {
      PHSensorSettings(uint8_t Pin = 0, float Slope = 0.0, float Intercept = 0.0) : Pin(Pin), Slope(Slope), Intercept(Intercept) {}
      uint8_t Pin;
      float Slope;
      float Intercept;
    };
    struct PHSensorSettings PHSensor1 = {.Pin = A3, .Slope = -0.033256, .Intercept = 24.08651}; ///Update this to your own PH meter calibration values

    struct PressureSensorSettings
    {
      PressureSensorSettings(uint8_t Pin = 0, float Offset = 0.0, float Ratio = 0.0) : Pin(Pin), Offset(Offset), Ratio(Ratio) {}
      uint8_t Pin;     ///Pressure sensor Pin: Signal(yellow)
      float Offset; ///Pressure sensor calibration: voltage reading at 0 pressure
      float Ratio;  ///Pressure sensor voltage to pressure ratio
    };
    struct PressureSensorSettings Pres1 = {.Pin = A1, .Offset = 0.57, .Ratio = 2.7};

    struct SoundSettings
    {
      SoundSettings(uint8_t Pin = 0) : Pin(Pin) {}
      uint8_t Pin;            ///PC buzzer+ (red)
      bool Enabled = true; ///Enable PC speaker / Piezo buzzer
    };
    struct SoundSettings Sound1 = {.Pin = 2};

    struct WaterLevelSensorSettings
    {
      WaterLevelSensorSettings(uint8_t Pin_1 = 0, uint8_t Pin_2 = 0, uint8_t Pin_3 = 0, uint8_t Pin_4 = 0) : Pin_1(Pin_1), Pin_2(Pin_2), Pin_3(Pin_3), Pin_4(Pin_4) {}
      uint8_t Pin_1; ///Lowest water level
      uint8_t Pin_2;
      uint8_t Pin_3;
      uint8_t Pin_4; ///Full
    };
    struct WaterLevelSensorSettings WaterLevel1 = {.Pin_1 = A4, .Pin_2 = A5, .Pin_3 = A6, .Pin_4 = A7};

    struct WaterPumpSettings
    {
      WaterPumpSettings(uint8_t PumpPin = 0, bool PumpEnabled = false, int PumpTimeOut = 0, uint8_t BypassSolenoidPin = 255, int PrimeTime = -1, int BlowOffTime = -1) : PumpPin(PumpPin), BypassSolenoidPin(BypassSolenoidPin), PumpEnabled(PumpEnabled), PumpTimeOut(PumpTimeOut), PrimeTime(PrimeTime), BlowOffTime(BlowOffTime)  {}
      uint8_t PumpPin;         ///< Pump relay pin
      uint8_t BypassSolenoidPin;        ///< Bypass solenoid relay pin
      bool PumpEnabled; ///< Enable/disable pump. false= Block running the pump
      int PumpTimeOut;   ///< (Sec) Max pump run time        
      int PrimeTime;    ///< (Sec) For wow long to keep the bypass solenoid on when starting the pump - Remove air bubbles from pump intake side
      int BlowOffTime;     ///< (Sec) For how long to open the bypass solenoid on after turning the pump off - Release pressure from pump discharge side
    };
    struct WaterPumpSettings HempyPump1 = {.PumpPin = 7, .PumpEnabled = true, .PumpTimeOut = 120}; ///< Submerged pumps do not need to prime at start or discharge pressure when stopping
    struct WaterPumpSettings HempyPump2 = {.PumpPin = 8, .PumpEnabled = true, .PumpTimeOut = 120}; ///< Submerged pumps do not need to prime at start or discharge pressure when stopping
    struct WaterPumpSettings AeroPump1 = {.PumpPin = 8, .BypassSolenoidPin = 23, .PumpEnabled = true, .PumpTimeOut = 120, .PrimeTime = 10, .BlowOffTime = 3};

    struct WaterTempSensorSettings
    {
      WaterTempSensorSettings(uint8_t Pin = 0) : Pin(Pin) {}
      uint8_t Pin;
    };
    struct WaterTempSensorSettings WaterTemp1 = {.Pin = 45}; ///Data(yellow) - DS18B20 waterproof temp sensor

    struct WeightSensorSettings
    {
      WeightSensorSettings(uint8_t DTPin = 0, uint8_t SCKPin = 0, float Scale = 0.0, long TareOffset = 0.0) : DTPin(DTPin), SCKPin(SCKPin), Scale(Scale), TareOffset(TareOffset) {}
      uint8_t DTPin;     ///Weight sensor DT pin
      uint8_t SCKPin; ///Weight sensor SCK pin
      float Scale;  ///Calibration scale value
      long TareOffset; ///Reading at 0 weight on the scale
    };
    struct WeightSensorSettings Weight1 = {.DTPin = 3, .SCKPin = 4, .Scale = 125000.0, .TareOffset=146000};
    struct WeightSensorSettings Weight2 = {.DTPin = 5, .SCKPin = 6, .Scale = 126000.0, .TareOffset=267461};

    /*   
    bool AutomaticIFan = false;  ///Adjust internal fan based on temperature
    bool AutomaticEFan = false;  ///Adjust exhaust fan based on temp and humidity
    int IFanSwitchTemp = 25; /// Above limit turn the internal fan to High, turn to Low if limit-3 degrees is reached. Has to match default unit type(Metric C or Imperial K)
    uint8_t EFanHighHumid = 65; ///Above set humidity turn exhaust fan High if automatic fan control is enabled
    uint8_t EFanLowHumid = 55; ///Above set humidity turn exhaust fan Low if automatic fan control is enabled
    uint8_t EFanOffHumid = 40; ///Below set humidity turn exhaust fan Off if automatic fan control is enabled
    */

    uint8_t CompatibilityVersion = Version; ///Should always be the last value stored.
  } Settings;

///////////////////////////////////////////////////////////////
///EEPROM related functions - Persistent storage between reboots
///Use cautiously, EEPROM has a write limit of 100.000 cycles - Only use these in the setup() function, or when a user initiated change is stored 

void saveSettings(Settings *ToSave);
Settings *loadSettings(bool ResetEEPROM = false );
void restoreDefaults(Settings *ToOverwrite);