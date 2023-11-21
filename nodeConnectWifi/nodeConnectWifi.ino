#include <ESP8266WiFi.h> //การประกาศเรียกใช้ Library ESP8266
const char* ssid = "Boom";
 //การประกาศสร้างตัวแปรเก็บ Username เครื่อข่าย WiFi ชื่อว่า ssid 
const char* pass = "A12345678";
 //การประกาศสร้างตัวแปรเก็บ Password ของเครื่อข่าย WiFi ชื่อว่า pass
void setup () {
  Serial.begin(9600); //
  WiFi.begin(ssid, pass); //ทำการ Connect SSID และ Pass
while (WiFi.status() != WL_CONNECTED) { // ถ้าไม่สามารถเชื่อมต่อได้
    // ทำการ Print "Connectiong..." ทุก 1000ms
    Serial.println("Connecting...  ");  
    // แสดงสถานะการเชื่อมต่อ
    Serial.printf("Connection Status: %d\n", WiFi.status()); 
    delay(1000);
  }
  // จะหลุดออกจาก while ก็ต่อเมือ Connected เรียบร้อย
  Serial.print("Wi-Fi connected."); 
  Serial.print("IP Address : ");
  Serial.println(WiFi.localIP());  // ทำการ Print IP ที่ได้รับมาจาก 
}
void loop () { 
  Serial.printf("Connection Status: %d\n", WiFi.status());// แสดงสถานะการเชื่อมต่อ
delay(2000);
}
