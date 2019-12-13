#define SEALEVELPRESSURE_HPA (1013.25)



Adafruit_BME280 bme; // I2C

void bme280Setup(){
  printDebug("Setting up BME280"); 

  Wire.begin();
  if (!bme.begin()) {
    printDebug("Could not find a valid BME280. Going to kill myself now."); 
    while (1); // suicide
  }

  bme.setSampling(Adafruit_BME280::MODE_SLEEP,
                      Adafruit_BME280::SAMPLING_X2, // temperature
                      Adafruit_BME280::SAMPLING_X2, // pressure
                      Adafruit_BME280::SAMPLING_X2, // humidity
                      Adafruit_BME280::FILTER_OFF   );
  
  printDebug("Setting up BME280 - Complete"); 
}

void bme280Measure(){
    printDebug("Measuring BME280"); 

    globalHumidity = bme.readHumidity();
    globalTemperature = bme.readTemperature();
    globalDewPoint = calcDewpoint(globalHumidity, globalTemperature);
    globalPressure = bme.readPressure()/100.0F;
    globalAltitude = bme.readAltitude(SEALEVELPRESSURE_HPA);

//  Serial.print("Temp:");Serial.println(globalTemperature, 3);
//  Serial.print("Humidity:");Serial.println(globalHumidity, 3);
//  Serial.print("Pressure:");Serial.println(globalPressure);
//  Serial.print("Altitude:");Serial.println(globalAltitude);
//  Serial.print("Dewpoint:");Serial.println(globalDewPoint);

    printDebug("Measuring BME280 - Complete"); 
}
