/**
   BasicHTTPClient.ino

    Created on: 24.05.2015

*/

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

#include <WiFiClient.h>
SoftwareSerial portOne(D7, D8); // RX,TX
ESP8266WiFiMulti WiFiMulti;

void setup() {
pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);
  portOne.begin(115200);
  // Serial.setDebugOutput(true);



  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    portOne.flush();
    delay(300);
  }

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("Depto 7", "wifi58195373");

}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);
  // wait for WiFi connection
  if ((WiFiMulti.run() == WL_CONNECTED)) {

    WiFiClient client;

    HTTPClient http;
    if (http.begin(client, "http://159.223.130.126:5000/test")) {  // HTTP

      // start connection and send HTTP header
      int httpCode = http.GET();

      // httpCode will be negative on error
      if (httpCode > 0) {
        // HTTP header has been send and Server response header has been handled

        // file found at server
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = http.getString();
          Serial.println(payload); 
          portOne.println(payload); 
        }
      } else {
 
      }
      http.end();
    } else {
      Serial.printf("[HTTP} Unable to connect\n");
    }
  }
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
}
