#include <Arduino.h> 
#include <ESP8266WiFi.h> 
#include <ESP8266mDNS.h> 
#include <ArduinoOTA.h> 
#include <ESPAsyncTCP.h> 
#include <ESPAsyncTCPbuffer.h>

const char* ssid= "pino";
const char* password= "password";

AsyncClient client1;

IPAddress ip(192,168,1,100);


void setup() {
  // put your setup code here, to run once:

    Serial.begin(115200);
    Serial.setDebugOutput(true);

    WiFi.mode(WIFI_STA);

    WiFi.begin(ssid, password);

    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
      Serial.printf("STA: Failed to connect!\nSTA: will retry in 1s...\n");
      WiFi.disconnect();
      delay(1000);
      WiFi.begin(ssid, password);
    }

    client1.connect(ip,3000);
    client1.onConnect(
      [](void* client, AsyncClient* cli){
      client1.write("1");
      Serial.println("1");
    },NULL);
    
   
    
    
}

void loop() {
  // put your main code here, to run repeatedly:

}
