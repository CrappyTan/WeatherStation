#define _PIN_ONE_WIRE_BUS D2


OneWire oneWire(_PIN_ONE_WIRE_BUS); 
DallasTemperature temperature_sensors(&oneWire);


void temperatureSetup(){
  temperature_sensors.begin();
  printDebug("Found " + String(temperature_sensors.getDeviceCount()) + " temperature sensors");
  
}


void temperatureMeasure(){
  temperature_sensors.requestTemperatures(); // Send the command to get temperature readings 
  globalTemperature = temperature_sensors.getTempCByIndex(0);
  printDebug("Temperature is: " + String(globalTemperature)); 
  
}
