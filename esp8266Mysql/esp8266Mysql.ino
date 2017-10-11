#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>

ESP8266WiFiMulti WiFiMulti;

const char* ssid     = "tom";
const char* password = "aaaaabbbb1";

const int analogInPin = A0;  // Analog input pin that the potentiometer is attached to
int sensorValue = 0;        // value read from the pot

WiFiClient client;


void setup() {

Serial.begin(115200);
if (WiFi.begin(ssid, password)) {
        while (WiFi.status() != WL_CONNECTED) {
            delay(500);
            Serial.print(".");
        }
    }

    Serial.println("WiFi connected");  
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

}
void loop() {
if ((WiFiMulti.run() == WL_CONNECTED)) {
HTTPClient http;
//int force = random(25,35);

sensorValue = analogRead(analogInPin);

  // print the results to the serial monitor:
  Serial.print("sensor = ");
  Serial.println(sensorValue);

String url = "http://rks.ac.th/iot/addesp.php?force="+String(sensorValue);
Serial.println(url);
http.begin(url); //HTTP

int httpCode = http.GET();
if (httpCode > 0) {
Serial.printf("[HTTP] GET... code: %d\n", httpCode);
if (httpCode == HTTP_CODE_OK) {
String payload = http.getString();
Serial.println(payload);
}
} else {
Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
}
http.end();
}
delay(1000);
}
