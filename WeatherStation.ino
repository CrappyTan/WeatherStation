#include <DHTesp.h>
#include <OneWire.h> 
#include <DallasTemperature.h>
#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h> 
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>


#define _DEBUG 1
#define _DO_MEASURE 1


#define _SLEEP_DURATION         300 //this is in seconds

#define _MQTT_HOST "mqtt.home"
#define _MQTT_HOST_PORT 1883
#define _MQTT_ROOT                               "homeQA/weather"
#define _MQTT_ROOT_WEATHER_HEARTBEAT             _MQTT_ROOT "/heartbeat"
#define _MQTT_ROOT_WEATHER_TEMPERATURE           _MQTT_ROOT "/temperature"
#define _MQTT_ROOT_WEATHER_HUMIDITY              _MQTT_ROOT "/humidity"
#define _MQTT_ROOT_WEATHER_BATTERY_VOLTAGE       _MQTT_ROOT "/battery/voltage"
#define _MQTT_ROOT_WEATHER_BATTERY_PERCENTAGE    _MQTT_ROOT "/battery/percentage"
#define _MQTT_ROOT_WEATHER_OTA_REMAINING         _MQTT_ROOT "/ota/remaining"
#define _MQTT_ROOT_WEATHER_OTA_START             _MQTT_ROOT "/ota/start"
#define _MQTT_ROOT_WEATHER_SYS_RUNTIME           _MQTT_ROOT "/sys/runtime"
#define _HOST_NAME                               "WeatherStation"
#define _WIFI_DEFAULT_PASSWORD                   "Qwe33r!y"


float globalBatteryVoltage  = 0;
int   globalBatteryPerc     = 0;
int   globalHumidity        = 0;
float globalTemperature     = 0;
bool  LED_STATE             = HIGH;

int sleepWaitFor = 0; //time to wait in ms before going to sleep. This is for OTA
unsigned long loopTimeSecond;



void printDebug(String msg){
  #ifdef _DEBUG
      Serial.print("[");
      Serial.print(millis());
      Serial.print("\t]\t");
      Serial.println(msg);    
  #endif
 }

void setup() {
    pinMode(BUILTIN_LED, OUTPUT);
    digitalWrite(BUILTIN_LED, LED_STATE);
    blinkLED();
  #ifdef _DEBUG
    Serial.begin(112500);
    Serial.println();
    Serial.println();
    printDebug(F("Hello"));
    printDebug(F("Weather Station - Starting up"));
  #endif
  

  printDebug(F("Setting up"));
  //setup
  wifiSetup();
  blinkLED();
  batterySetup();
  blinkLED();
  mqttSetup();
  blinkLED();
  dht11Setup();
  blinkLED();
  temperatureSetup();
  blinkLED();
  otaSetup();
  blinkLED();
  printDebug(F("Setting up - Complete"));

  
  printDebug(F("Measuring"));
  //measure sensors
  batteryMeasure();
  blinkLED();
  dht11Measure();
  blinkLED();
  temperatureMeasure();
  blinkLED();
  printDebug(F("Measuring - Complete"));
  

  printDebug(F("Sending MQTT"));
 //send values
  mqttSend();
  blinkLED();
  MQTT_loop(); 
  blinkLED();
  printDebug(F("Sending MQTT - Complete"));

  printDebug(F("setup - Done"));
  
}

void loop() {
  // put your main code here, to run repeatedly:
  MQTT_connect();
  MQTT_loop();

  
  if (sleepWaitFor == 0){
    printDebug(F("Complete."));
    mqttSendFinal();
    mqttDisconnect();
    printDebug(F("Going to sleep."));
    ESP.deepSleep(_SLEEP_DURATION * 1000000);
  }
  
   
  if((millis() - loopTimeSecond) > 1000) // Only check every second
  { 
    //loop_second();
    blinkLED();
    sleepWaitFor -= 1; //dec the timer.
    mqttSendOTATimeRemaining();
    loopTimeSecond = millis();
  }
  
  ArduinoOTA.handle();
 
}

void blinkLED(){
    digitalWrite(BUILTIN_LED, LED_STATE);
    LED_STATE = !LED_STATE;
}
