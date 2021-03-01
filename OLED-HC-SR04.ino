#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define OLED_RESET 16
Adafruit_SSD1306 display(OLED_RESET);

const int pingPin = D3; //trig
int inPin = D4; //echo
int led = D6;
 
void setup() {
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3c); //สั่งให้จอ OLED เริ่มทำงานที่ Address 0x3C
  display.clearDisplay(); // ลบภาพในหน้าจอทั้งหมด
 // display.setTextSize(1); // กำหนดขนาดตัวอักษร
  display.setTextColor(WHITE);
  display.setCursor(0,0); // กำหนดตำแหน่ง x,y ที่จะแสดงผล
  display.println(" OLED 0.96 TESTER ");
  display.setCursor(0,10);
  //display.setTextSize(1);
  display.setTextColor(BLACK, WHITE); //กำหนดข้อความสีขาว ฉากหลังสีดำ
  display.println(" Technocom1");
  display.display();
  delay(1000);
  display.clearDisplay();
}
 
void loop()
{
  long duration, cm;
  pinMode(pingPin, OUTPUT); 
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);
  pinMode(inPin, INPUT);
  duration = pulseIn(inPin, HIGH);
  cm = microsecondsToCentimeters(duration);
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();
  display.setTextColor(WHITE,BLACK); // Draw white text
  display.setCursor(10,10); // กำหนดตำแหน่ง x,y ที่จะแสดงผล
  display.print("Distance");
  display.setCursor(10,20); // กำหนดตำแหน่ง x,y ที่จะแสดงผล
  display.print(cm);
  display.print(" cm ");//เคลียตัวหนังสือไม่ให้แสดงค้าง
  display.display();
if (cm < 10) {
digitalWrite(led, HIGH);
}
else {
digitalWrite(led, LOW);
Serial.println(" cm");
}
delay(100);
 
}
 
long microsecondsToCentimeters(long microseconds)
{
// The speed of sound is 340 m/s or 29 microseconds per centimeter.
// The ping travels out and back, so to find the distance of the
// object we take half of the distance travelled.
return microseconds / 29 / 2;
}
