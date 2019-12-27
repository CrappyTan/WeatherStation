

void wifiSetup(){
    printDebug(F("WiFI setup starting"));

  
    WiFi.mode(WIFI_STA);
    unsigned long wifiConnectStart = millis();
    WiFi.hostname(_HOST_NAME); 

    
    while (WiFi.status() != WL_CONNECTED) {
      if (WiFi.status() == WL_CONNECT_FAILED) {
        printDebug(F("WiFI connection failed"));
        return;
      }
      if (millis() - wifiConnectStart > 5000) {
        WiFiManager wifiManager;
        
        printDebug(F("Could not connect with normal wifi. Starting WiFi Manager."));
        wifiManager.autoConnect(_HOST_NAME);
      }
      delay(10);
    }
//Serial.print("mac:");
//Serial.println(WiFi.softAPmacAddress());
    globalSystemRSSI = WiFi.RSSI();
    printDebug("RSSI" + String(globalSystemRSSI));    
    printDebug(F("WiFI setup complete"));    
}
