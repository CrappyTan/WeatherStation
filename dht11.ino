
DHTesp dht;
int dhtPin = D1;


void dht11Setup(){
  dht.setup(dhtPin, DHTesp::DHT11);
  delay(10);//Delay to let system boot
  
}


void dht11Measure(){
    float humidity = dht.getHumidity();
    globalHumidity = (humidity * 100) / 100.00;
    printDebug("Humidity is: " + String(globalHumidity)); 
}
