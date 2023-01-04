#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPLtJlSJeSg"
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).

#define BLYNK_DEVICE_NAME "led"
#define BLYNK_AUTH_TOKEN "kCCqs__pOaRAzzPmeQF4800xwNoFdcIT"

char auth[] = "kCCqs__pOaRAzzPmeQF4800xwNoFdcIT";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Guitar-2.4";
char pass[] = "AAAAABBBB1";

// Select the digital pin that the LED is connected to
const int ledPin1 = D4; //GPIO2 
const int ledPin2 = D0; //GPIO16

void setup() {
  // Initialize the serial console
  Serial.begin(115200);

  // Connect to WiFi
  Blynk.begin(auth, ssid, pass);

  // Initialize the LED pin as an output
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  digitalWrite(ledPin1, HIGH);
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
  Blynk.run();
}
