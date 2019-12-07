//Robo India tutorial On Controlling LED on WEB 
//https://www.roboindia.com/tutorials


#include <ESP8266WiFi.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

const char* ssid = "Tomwifi";
const char* password = "aaaaabbbb1";
 
int LED = 14;                 // led connected to D0
WiFiServer server(80);

LiquidCrystal_I2C lcd(0x3F, 16, 2);

void setup() 
{
  // initialize the LCD
  lcd.begin();
  // Turn on the blacklight and print a message.
  lcd.backlight();
  lcd.print("Control Device");
  lcd.setCursor(0, 1);
  lcd.print("On WEB by TOM");
  delay (5000);
  
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
 
  Serial.print("Connecting to Internet ");
  lcd.setCursor(0, 0);
  lcd.print("Connecting to Internet");
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
    lcd.setCursor(0, 0);
    lcd.print(".");
  }
  Serial.println("WiFi connected");
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print(" WiFi connected ");
 
 /*-------- server started---------*/ 
  server.begin();
  Serial.println("Server started");
 
  /*------printing ip address--------*/
  Serial.print("IP Address of network: ");
  Serial.println(WiFi.localIP());
  lcd.setCursor(2, 1);
  lcd.print(WiFi.localIP());
  Serial.print("Copy and paste the following URL: https://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
}
 
void loop() 
  {
    WiFiClient client = server.available();    
    if (!client) 
    {
      return;
    }
  Serial.println("Waiting for new client");   
  while(!client.available())
  {
    delay(1);
  }
 
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
 
 
  int value = LOW;
  if (request.indexOf("/LED=ON") != -1)  
  {
    digitalWrite(LED, HIGH);
    value = HIGH;
  }
  if (request.indexOf("/LED=OFF") != -1)  
  {
    digitalWrite(LED, LOW);
    value = LOW;
  }
 
/*------------------Creating html page---------------------*/

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); 
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
 
  client.print("LED is: ");
 
  if(value == HIGH) 
  {
    client.print("ON");
  } 
  else 
  {
    client.print("OFF");
  }
  client.println("<br><br>");
  client.println("<a href=\"/LED=ON\"\"><button>ON</button></a>");
  client.println("<a href=\"/LED=OFF\"\"><button>OFF</button></a><br />");  
  client.println("</html>");
 
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");
 
}
