#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup(){
  Wire.begin();
  display.init(); // สั่งให้จอ Oled เริ่มทำงาน
  display.clearDisplay(); // ล้างภาพหน้าจอ
  display.setTextXY(0,0); // เลื่อน cursor ไปที่บรรทัดที่ 0 ตำแหน่งที่ 0
  display.putString("Welcome To");
  display.setTextXY(1,5); // เลื่อน cursor ไปที่บรรทัดที่ 1 ตำแหน่งที่ 5
  display.putString("ArduinoALL");
  display.setTextXY(3,0); // เลื่อน cursor ไปที่บรรทัดที่ 3 ตำแหน่งที่ 0
  display.putString("ESP8266 NodeMCu");
  display.setTextXY(4,0); // เลื่อน cursor ไปที่บรรทัดที่ 4 ตำแหน่งที่ 10
  display.putString("Tutor");
}

void loop(){
}
