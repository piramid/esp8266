#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID " "
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include "DHT.h"

char auth[] = "";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "";
char pass[] = "";

#define DHTPIN D3     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE,15);

const int ledPin1 = D4; //GPIO2 
const int ledPin2 = D0; //GPIO16

void setup() {
  Serial.begin(115200); 
  Serial.println("DHTxx test!");
  dht.begin();
  Blynk.begin(auth, ssid, pass);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  digitalWrite(ledPin1, HIGH); // ในnodemcu LED ดับ เมื่อ HIGH ติด เมื่อ LOW
  digitalWrite(ledPin2, HIGH); 
}

BLYNK_WRITE(V1) {
  // Read the value of the Button widget
  int buttonValue = param.asInt();


  // If the button is pressed, turn the LED on; otherwise, turn it off
  if (buttonValue == 1) {
    digitalWrite(ledPin1, LOW);
  } else {
    digitalWrite(ledPin1, HIGH);
  }
}

BLYNK_WRITE(V2) {
  // Read the value of the Button widget
  int buttonValue = param.asInt();

  // If the button is pressed, turn the LED on; otherwise, turn it off
  if (buttonValue == 1) {
    digitalWrite(ledPin2, LOW);
  } else {
    digitalWrite(ledPin2, HIGH);
  }
}

void loop() {
  // Wait a few seconds between measurements.
  delay(1000);
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius
  float t = dht.readTemperature();
  
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // show in serial monitor
  Serial.print("Humidity: "); 
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: "); 
  Serial.print(t);
  Serial.print(" *C \n");

  Blynk.run();
  delay(100);
  Blynk.virtualWrite(V3, t);
  Blynk.virtualWrite(V4, h);
  
}
