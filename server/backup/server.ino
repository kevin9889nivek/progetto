#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
#include <ESPAsyncTCP.h>
 
#define LEDPin 2
 
 
const char* ssid= "TNCAP993B7D";
const char* password= "DFC3B98A7A";
 
 
const char *ap_ssid = "pino";
const char *ap_password = "password";
 
AsyncClient *clients[7];
 
AsyncServer server(3000);
 
 
IPAddress ip(192, 168, 2, 100);
IPAddress gateway(192, 168, 2, 254);
IPAddress subnet(255, 255, 255, 0);
 
 
uint8_t toInt(char numero) {
 
    return (numero - 48);
}
 
void setup() {
    // put your setup code here, to run once:
 
    Serial.begin(115200);
    Serial.setDebugOutput(true);
 
    WiFi.mode(WIFI_AP_STA);
 
    WiFi.begin(ssid, password);
 
    WiFi.softAP(ap_ssid, ap_password);
 
    WiFi.softAPConfig(ip, gateway, subnet);
 
 
    while (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Serial.println("Connection Failed! Rebooting...");
        delay(5000);
        ESP.restart();
    }
    ArduinoOTA.begin();
 
    server.onClient(
            [](void *server, AsyncClient *client) {
                Serial.printf("TCP: Client connected\n");
                client->onData(
                        [](void *null, AsyncClient *client, void *data, size_t len) {
                            Serial.printf("Dato ricevuto\n");
 
                            //uint8_t number=toInt(((char*)data)[0]);
                            uint8_t *number = (uint8_t *) malloc(sizeof(uint8_t));
                            *number = toInt(((char *) data)[0]);
                            clients[*number] = client;
 
                            Serial.printf("Collegato: %i\n", *number);
 
 
                            client->onData(
                                    [](void *number, AsyncClient *client, void *data, size_t len) {
 
                                        Serial.printf("Data from %i: ", *(char *) number);
                                        for (unsigned char i = 0; i < len; ++i) {
 
                                            Serial.print(((char *) data)[i]);
                                        }
 
                                        Serial.println();
                                    }, number
                            );
                        },
 
                        client);
            }, NULL);
 
    server.begin();
}
 
 
void loop() {
    // put your main code here, to run repeatedly:
    ArduinoOTA.handle();
}
