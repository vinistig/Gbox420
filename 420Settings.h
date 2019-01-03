//Default settings of the grow box

//Global constants
  const char PushingBoxLogRelayID[]= "v755877CF53383E1"; //UPDATE THIS to your grow box logging scenario DeviceID from PushingBox
  const char PushingBoxEmailAlertID[]  = "vC5244859A2453AA";  //UPDATE THIS to your email alert scenario DeviceID from PushingBox
  const byte ScreenRotation = 1;  //LCD screen rotation: 1,3:landscape 2,4:portrait
  const byte PotStepping = 100;  // Digital potentiometer adjustment steps
  const int AeroPumpTimeout = 600000;  // Aeroponics - Max pump run time (10minutes)
  const byte LogDepth = 8;  //Show X log entries on website
  const byte LogLength = 31;  //30 characters + null terminator for one log entry
    
//Settings saved to EEPROM persistent storage
  byte Version= 1; //increment this when you update the test values or change the stucture to invalidate the EEPROM stored settings
  struct SettingsStructure //when Version is changed these values get stored in EEPROM, else EEPROM content is loaded
  {
  byte AeroInterval = 15; //Aeroponics - Spray every 15 minutes
  byte AeroDuration = 3; //Aeroponics - Spray for 5 secondsf
  float AeroPressureLow= 5.5; //Aeroponics - Turn on pump below this pressure (bar)
  float AeroPressureHigh = 7.0 ; //Aeroponics - Turn on pump below this pressure (bar)
  float AeroOffset = 0.5; //Pressure sensor calibration - offset voltage
  bool isAeroSprayEnabled = true;  //Enable/disable misting
  bool isAeroQuietEnabled = true;  //Enable/disable quiet time
  bool AeroRefillBeforeQuiet = true; //Enable/disable refill before quiet time
  byte AeroQuietFromHour = 21;  //Quiet time to block pump - hour
  byte AeroQuietFromMinute = 00; //Quiet time to block pump - minute
  byte AeroQuietToHour = 9; //Quiet time end - hour
  byte AeroQuietToMinute = 00; //Quiet time end - minute
  
  bool isLightOn = true;  //Startup status for lights: True-ON / False-OFF
  byte LightBrightness; //0 - 100 range for controlling led driver output  
  byte LightOnHour = 4;  //Light ON time - hour
  byte LightOnMinute = 20; //Light ON time - minute
  byte LightOffHour = 16; //Light OFF time - hour
  byte LightOffMinute = 20; //Light OFF time - minute
  bool isTimerEnabled = false;  //Enable timer controlling lights  

  bool isInternalFanOn;  //Internal fan On/Off
  bool isInternalFanHigh; //Internal fan Low/High
  bool isExhaustFanOn;  //Exhaust fan On/Off
  bool isExhaustFanHigh;  //Exhaust fan Low/High  
  bool isPCPowerSupplyOn = true;  //Startup status for PC power supply: True-ON / False-OFF 
  
  bool ReportToGoogleSheets = true;
  bool ReportToMqtt = true;
  
  byte DigitDisplayBacklight = 75; //4 digit display - backlight strenght (0-100)
  bool isSoundEnabled = true;  //Enable PC speaker  
  byte StructureVersion = Version;
  };

  typedef struct SettingsStructure settings;  //create the "settings" type using the stucture
  settings MySettings;  //create a variable of type "settings"  with default values from SettingsStructure
