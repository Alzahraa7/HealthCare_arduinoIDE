#include <ArduinoJson.h>
#include <Firebase.h>
#include <FirebaseArduino.h>
#include <FirebaseCloudMessaging.h>
#include <FirebaseError.h>
#include <FirebaseHttpClient.h>
#include <ESP8266HTTPClient.h>
#include <FirebaseObject.h>
#include <BearSSLHelpers.h>
#include <CertStoreBearSSL.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiAP.h>
#include <ESP8266WiFiGeneric.h>
#include <ESP8266WiFiGratuitous.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFiScan.h>
#include <ESP8266WiFiSTA.h>
#include <ESP8266WiFiType.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiClientSecureBearSSL.h>
#include <WiFiServer.h>
#include <WiFiServerSecure.h>
#include <WiFiServerSecureBearSSL.h>
#include <WiFiUdp.h>

#include <BearSSLHelpers.h>
#include <CertStoreBearSSL.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiAP.h>
#include <ESP8266WiFiGeneric.h>
#include <ESP8266WiFiGratuitous.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFiScan.h>
#include <ESP8266WiFiSTA.h>
#include <ESP8266WiFiType.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiClientSecureBearSSL.h>
#include <WiFiServer.h>
#include <WiFiServerSecure.h>
#include <WiFiServerSecureBearSSL.h>
#include <WiFiUdp.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// Define to which pin of the Arduino the 1-Wire bus is connected:
#define ONE_WIRE_BUS D3

// Create a new instance of the oneWire class to communicate with any OneWire device:
OneWire oneWire(ONE_WIRE_BUS);

// Pass the oneWire reference to DallasTemperature library:
DallasTemperature sensors(&oneWire);
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include "MAX30105.h"
#include "heartRate.h"
#include "spo2_algorithm.h"
#define sens A0
#define MAX_BRIGHTNESS 255
#define btemp D5
#define bhr D7
#define bspo D6

#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
//Arduino Uno doesn't have enough SRAM to store 100 samples of IR led data and red led data in 32-bit format
//To solve this problem, 16-bit MSB of the sampled data will be truncated. Samples become 16-bit data.
uint16_t irBuffer[100]; //infrared LED sensor data
uint16_t redBuffer[100];  //red LED sensor data
#else
uint32_t irBuffer[100]; //infrared LED sensor data
uint32_t redBuffer[100];  //red LED sensor data
#endif

int32_t bufferLength; //data length
int32_t spo2; //SPO2 value
int8_t validSPO2; //indicator to show if the SPO2 calculation is valid
int32_t heartRate; //heart rate value
int8_t validHeartRate; //indicator to show if the heart rate calculation is valid
int result;
int result1;
int result2;
// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int buzzer= D8;
MAX30105 particleSensor;
const byte RATE_SIZE = 4; //Increase this for more averaging. 4 is good.
byte rates[RATE_SIZE]; //Array of heart rates
byte tempSpot = 0;
byte rateSpot = 0;
long lastBeat = 0; //Time at which the last beat occurred
float beatsPerMinute;
int beatAvg;
int value =0;
float volt =0;
float temp =0;
int count = 0 ;
int count1 = 0 ;
int tempState = 0 ;
int spoState = 0 ;
int hrState = 0 ;
int spotemp = 0;
int hrtemp = 0;
long t=0;
long duration =0;
LiquidCrystal_I2C lcd(0x27, 16, 4);

const char* ssid="Infinix Smart 2 Pro";
const char* password = "sarasara";
const char* firebase_host ="elinic-gp2021-default-rtdb.firebaseio.com";
const char* firebase_auth="6BULIbxyPYprAWVbpYm63phXdpt5jUQOmeCrgrhT";

void setup() {
  Serial.begin(115200);
  Wire.begin(4,5);
  lcd.begin(16,4);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("Health Care");
  lcd.setCursor(0, 1);
  lcd.print("Hold sensor 1min");
  pinMode(btemp, INPUT_PULLUP);
  pinMode(bhr, INPUT_PULLUP);
  pinMode(bspo, INPUT_PULLUP);
   result = digitalRead(btemp);
   result1 = digitalRead(bhr);
   result2 = digitalRead(bspo);
  // Initialize sensor
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) //Use default I2C port, 400kHz speed
  {
    Serial.println("MAX30105 was not found. Please check wiring/power. ");
    while (1);
  }
  Serial.println("Place your index finger on the sensor with steady pressure.");

  
  particleSensor.setPulseAmplitudeRed(0x0A); //Turn Red LED to low to indicate sensor is running
  particleSensor.setPulseAmplitudeGreen(0); //Turn off Green LED
  byte ledBrightness = 60; //Options: 0=Off to 255=50mA
  byte sampleAverage = 4; //Options: 1, 2, 4, 8, 16, 32
  byte ledMode = 2; //Options: 1 = Red only, 2 = Red + IR, 3 = Red + IR + Green
  byte sampleRate = 100; //Options: 50, 100, 200, 400, 800, 1000, 1600, 3200
  int pulseWidth = 411; //Options: 69, 118, 215, 411
  int adcRange = 4096; //Options: 2048, 4096, 8192, 16384

  particleSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange); //Configure sensor with these settings
  t= millis();
  Serial.begin(115200);
  Serial.println();
  Serial.print("Wifi connecting to ");
  Serial.println( ssid );

  WiFi.begin(ssid,password);

  Serial.println();
  Serial.print("Connecting");

  while( WiFi.status() != WL_CONNECTED ){
      delay(500);
      Serial.print(".");        
  }

  Serial.println("Wifi Connected Success!");
  Serial.print("NodeMCU IP Address : ");
  Serial.println(WiFi.localIP() );
  Firebase.begin(firebase_host,firebase_auth);
}

void loop() {
  int btn = digitalRead(bhr);
  if (btn == HIGH){
    tempState=0;
    spoState=0;
    hrState=1;
    heratbeat();
  } 
  int btn1 = digitalRead(btemp);
  if (btn1 == HIGH){
    tempState=1;
    spoState=0;
    hrState=0;
    LM35();
  }
  int btn2 = digitalRead(bspo);
  if (btn2 == HIGH){
    tempState=0;
    spoState=1;
    hrState=0;
    spo();
    Serial.println("hello");
  }
  if (btn == LOW & btn1 == LOW & btn2 == LOW){
    lcd.setCursor(-4, 3);
    lcd.print("Waiting Sensor..");
    tempState=0;
    hrState=0;
    spoState=0;
  }
}
