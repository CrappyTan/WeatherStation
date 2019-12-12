#include <PubSubClient.h>

WiFiClient espClient;

PubSubClient psClient(espClient);

void mqttSetup(){
  psClient.setServer(_MQTT_HOST, _MQTT_HOST_PORT);
  psClient.setCallback(getMQTTPacket);
  MQTT_connect();
  
  printDebug(F("Setting up MQTT Complete"));
}

void getMQTTPacket(char* topic, byte* payload, unsigned int length){
    
    char*  pl = (char *)payload;
    pl[length] = '\0';
    
    if (strcmp(pl, "ON") == 0) {
          printDebug(F("Found incoming message. Starting OTA timer"));
          psClient.publish(_MQTT_ROOT_WEATHER_OTA_START, "OFF", true); //publish retained OFF in acknowledgment.
          psClient.publish(_MQTT_ROOT_WEATHER_OTA_REMAINING, "0"); 
          sleepWaitFor = 60;
    }
}

void mqttSendFinal(){
  char s[20];
  sprintf(s, "%d", millis());
  psClient.publish(_MQTT_ROOT_WEATHER_SYS_RUNTIME, s);
  
}

void mqttSendOTATimeRemaining(){
  printDebug("OTA Time Remaining:" + String(sleepWaitFor));
  psClient.publish(_MQTT_ROOT_WEATHER_OTA_REMAINING, String(sleepWaitFor).c_str());  
}

void MQTT_loop(){
  psClient.loop();
}

void MQTT_connect() {
  if (psClient.connected()) {
    
    return;
  }

  unsigned int retries = 3;
  while (!psClient.connected()) {
    printDebug(F("Attempting MQTT connection..."));  
    // Attempt to connect
    if (psClient.connect(_HOST_NAME)) {
      printDebug(F("MQTT connected!"));
      psClient.subscribe(_MQTT_ROOT_WEATHER_OTA_START);
      psClient.publish(_MQTT_ROOT_WEATHER_HEARTBEAT, "ON");
    } 
    else {
      if (retries == 0){
        printDebug(F("Failed to connect to PSClient. Commiting suicide."));
        while(true); //WD will reset us.
      }
      printDebug(F("Failed to connect to PSClient. Waiting 1 seconds."));
      // Wait 5 seconds before retrying
      delay(1000);
      retries--;
    }
  }
  printDebug(F("Leaving PS CLient connect"));
}

void mqttSend(){
  psClient.publish(_MQTT_ROOT_WEATHER_BATTERY_VOLTAGE, String(globalBatteryVoltage).c_str(), true);
  psClient.publish(_MQTT_ROOT_WEATHER_BATTERY_PERCENTAGE, String(globalBatteryPerc).c_str(), true);
  psClient.publish(_MQTT_ROOT_WEATHER_HUMIDITY, String(globalHumidity).c_str(), true);
  psClient.publish(_MQTT_ROOT_WEATHER_TEMPERATURE, String(globalTemperature).c_str(), true);
}


void mqttDisconnect(){
  printDebug(F("Disconnecting from MQTT."));
  if (!psClient.connected()) {
    printDebug(F("MQTT was not connected.")); 
    return;
  }
  psClient.disconnect();
  printDebug(F("MQTT disconnected."));
}
