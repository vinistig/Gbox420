//GrowBoxGuy - http://sites.google.com/site/growboxguy/
//Sketch for testing: 8 channel relay module

//Pins
const byte Relay1OutPin = 22; //Power relay Port 1
const byte Relay2OutPin = 23; //Power relay Port 2
const byte Relay3OutPin = 24; //Power relay Port 3
const byte Relay4OutPin = 25; //Power relay Port 4
const byte Relay5OutPin = 26; //Power relay Port 5
const byte Relay6OutPin = 27; //Power relay Port 6
const byte Relay7OutPin = 28; //Power relay Port 7
const byte Relay8OutPin = 29; //Power relay Port 8

void setup()
{ // put your setup code here, to run once:
    Serial.begin(115200);
    pinMode(Relay1OutPin, OUTPUT);
    pinMode(Relay2OutPin, OUTPUT);
    pinMode(Relay3OutPin, OUTPUT);
    pinMode(Relay4OutPin, OUTPUT);
    pinMode(Relay5OutPin, OUTPUT);
    pinMode(Relay6OutPin, OUTPUT);
    pinMode(Relay7OutPin, OUTPUT);
    pinMode(Relay8OutPin, OUTPUT);

    digitalWrite(Relay1OutPin, HIGH);
    digitalWrite(Relay2OutPin, HIGH);
    digitalWrite(Relay3OutPin, HIGH);
    digitalWrite(Relay4OutPin, HIGH);
    digitalWrite(Relay5OutPin, HIGH);
    digitalWrite(Relay6OutPin, HIGH);
    digitalWrite(Relay7OutPin, HIGH);
    digitalWrite(Relay8OutPin, HIGH);

    delay(5000);
    Serial.println(F("Turning Relay1 ON"));
    digitalWrite(Relay1OutPin, LOW);
    delay(5000);
    Serial.println(F("Turning Relay2 ON"));
    digitalWrite(Relay2OutPin, LOW);
    delay(5000);
    Serial.println(F("Turning Relay3 ON"));
    digitalWrite(Relay3OutPin, LOW);
    delay(5000);
    Serial.println(F("Turning Relay4 ON")); //NOTE: The relay module will not get enough power after turning on the 4th relay if it is powered from the Arduino
    digitalWrite(Relay4OutPin, LOW);        //To operate a relay on 5V you need 60mA per relay. Arduino Mega can only supply 200mA max on VCC, limiting the number of simultaneously active relays to 3.
    delay(5000);
    Serial.println(F("Turning Relay5 ON"));
    digitalWrite(Relay5OutPin, LOW);
    delay(5000);
    Serial.println(F("Turning Relay6 ON"));
    digitalWrite(Relay6OutPin, LOW);
    delay(5000);
    Serial.println(F("Turning Relay7 ON"));
    digitalWrite(Relay7OutPin, LOW);
    delay(5000);
    Serial.println(F("Turning Relay8 ON"));
    digitalWrite(Relay8OutPin, LOW);
}

void loop()
{ // put your main code here, to run repeatedly:
    delay(5000);
    Serial.println(F("Turning Relay1 ON"));
    digitalWrite(Relay8OutPin, HIGH);
    digitalWrite(Relay1OutPin, LOW);
    delay(5000);
    Serial.println(F("Turning Relay2 ON"));
    digitalWrite(Relay1OutPin, HIGH);
    digitalWrite(Relay2OutPin, LOW);
    delay(5000);
    Serial.println(F("Turning Relay3 ON"));
    digitalWrite(Relay2OutPin, HIGH);
    digitalWrite(Relay3OutPin, LOW);
    delay(5000);
    Serial.println(F("Turning Relay4 ON"));
    digitalWrite(Relay3OutPin, HIGH);
    digitalWrite(Relay4OutPin, LOW);
    delay(5000);
    Serial.println(F("Turning Relay5 ON"));
    digitalWrite(Relay4OutPin, HIGH);
    digitalWrite(Relay5OutPin, LOW);
    delay(5000);
    Serial.println(F("Turning Relay6 ON"));
    digitalWrite(Relay5OutPin, HIGH);
    digitalWrite(Relay6OutPin, LOW);
    delay(5000);
    Serial.println(F("Turning Relay7 ON"));
    digitalWrite(Relay6OutPin, HIGH);
    digitalWrite(Relay7OutPin, LOW);
    delay(5000);
    Serial.println(F("Turning Relay8 ON"));
    digitalWrite(Relay7OutPin, HIGH);
    digitalWrite(Relay8OutPin, LOW);
}
