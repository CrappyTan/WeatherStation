#define _PIN_BATTERY    A0
#define _REF_VOLTAGE    4.65629
#define _REF_BATT_LOW   280
#define _REF_BATT_HIGH  420





void batterySetup(){
  pinMode(_PIN_BATTERY, INPUT);
}


void batteryMeasure(){
  unsigned int raw = analogRead(_PIN_BATTERY);
  float volt=(raw/1024.0) * _REF_VOLTAGE;
  globalBatteryPerc     = constrain(map((int)(volt * 100), _REF_BATT_LOW, _REF_BATT_HIGH, 0, 100), 0, 100);
  globalBatteryVoltage  = (volt * 100) / 100.00;
  printDebug("Battery voltage is: " + String(globalBatteryVoltage) + "V");
  printDebug("Battery percentage is: " + String(globalBatteryPerc) + "%");

 
}
