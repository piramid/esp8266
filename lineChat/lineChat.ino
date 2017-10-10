void Line_Notify(String messageOn) ;

#include <ESP8266WiFi.h>

// Config connect WiFi
#define WIFI_SSID "tom"
#define WIFI_PASSWORD "aaaaabbbb1"

// Line config
#define LINE_TOKEN "GhUpuJaEcl5e5KNKgFJTnE6eBlapxH9nRM9bUXuihmr"

#define SW D6

String messageOn = "มีการกดสวิทย์";

void setup() {
  pinMode(SW, INPUT);

  Serial.begin(9600);

  WiFi.mode(WIFI_STA);
  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if (digitalRead(SW) == HIGH) {
    while(digitalRead(SW) == HIGH) delay(10);

    Serial.println("Enter !");
    Line_Notify(messageOn);
    // Serial.println();
  }
  delay(10);
}

void Line_Notify(String messageOn) {
  WiFiClientSecure client;

  if (!client.connect("notify-api.line.me", 443)) {
    Serial.println("connection failed");
    return;   
  }

  String req = "";
  req += "POST /api/notify HTTP/1.1\r\n";
  req += "Host: notify-api.line.me\r\n";
  req += "Authorization: Bearer " + String(LINE_TOKEN) + "\r\n";
  req += "Cache-Control: no-cache\r\n";
  req += "User-Agent: ESP8266\r\n";
  req += "Content-Type: application/x-www-form-urlencoded\r\n";
  req += "Content-Length: " + String(String("message=" + messageOn).length()) + "\r\n";
  req += "\r\n";
  req += "message=" + messageOn;
  // Serial.println(req);
  client.print(req);
    
  delay(20);

  // Serial.println("-------------");
  while(client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      break;
    }
    //Serial.println(line);
  }
  // Serial.println("-------------");
}
