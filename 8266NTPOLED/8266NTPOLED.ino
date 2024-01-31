// ESP8266 NTP TIME :: http://fitrox.lnwshop.com
// ESP8266 NTP Client Exm.1


#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
// OLED Library
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 16
Adafruit_SSD1306 display(OLED_RESET);
//SCL to D1 SDA to D2
// set ค่า wifi เป็นของตัวเอง
const char *ssid     = "Tccar";
const char *password = "aaaaabbbbb";

// กำหนดค่า offset time เนื่องจากเวลาของเซิฟเวอร์นี้เป็นเวลา UTC เราต้องทำให้เป็นเวลาของประเทศไทย
// เวลาของประเทศไทย = UTC+7 ชั่วโมง ต้องกำหนด offset time = 7 ชั่วโมง
const long offsetTime = 25200;       // หน่วยเป็นวินาที จะได้ 7*60*60 = 25200

// กำหนด object ของ WiFiUDP ชื่อว่า ntpUDP
WiFiUDP ntpUDP;

// กำหนด object ของ NTPClient ชื่อว่า timeClient มีรูปแบบ ("WiFiUDP Object","NTP Server Address","offset time")
NTPClient timeClient(ntpUDP, "pool.ntp.org", offsetTime);

// กำหนดตัวแปรเก็บค่าเวลา ชั่วโมง-นาที-วินาที
int hourNow, minuteNow, secondNow;

void showTime(int h,int m, int s) {
// text display tests
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print(h);
  display.print(":");
  display.print(m);
  display.print(":");
  display.print(s);
  display.display();
  display.clearDisplay();
}

  
void setup(){
// เริ่มใช้งาน Serial
  Serial.begin(115200);
// เขื่อมต่อกับ WiFi 
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // set display
  display.begin(SSD1306_SWITCHCAPVCC, 0x78>>1);
  display.display();
  delay(2000);
  display.clearDisplay();
  
// เริ่มการทำงานของ NTPClient
  timeClient.begin();
}


void loop() {
// ร้องขอ timestamps ด้วยคำสั่ง update
  timeClient.update();
// get ค่าวินาที นาที ชั่วโมง ไปเก็บไว้ในตัวแปร
  secondNow = timeClient.getSeconds();
  minuteNow = timeClient.getMinutes();
  hourNow = timeClient.getHours();
// แสดงผลทาง Serial monitor รูปแบบ hh:mm:ss หากตัวไหนมีหลักเดียวให้เติม 0 ด้านหน้า ให้ครบ 2 หลักเสมอ
  if(hourNow < 10)(Serial.print("0"));
  Serial.print(hourNow);
  Serial.print(":");
  if(minuteNow < 10)(Serial.print("0"));
  Serial.print(minuteNow);
  Serial.print(":");
  if(secondNow < 10)(Serial.print("0"));
  Serial.print(secondNow);
  Serial.println();
  showTime(hourNow, minuteNow, secondNow);
// หน่วงเวลา 1 วินาที
  delay(1000);
}
