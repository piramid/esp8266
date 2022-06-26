#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F, 16, 2); //adress 0x3F col=16 row=2

void setup() {
  Wire.begin(2, 0);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("LCD1602 I2C Test");
  lcd.setCursor(0,1);
  lcd.print("Pornchai");
}

void loop() {
  // put your main code here, to run repeatedly:

}
