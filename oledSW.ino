#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

int sw1 = D3;
int led1 = D6;
int swState = 0;

void setup() {
  Serial.begin(9600);
  pinMode(sw1, INPUT);
  pinMode(led1, OUTPUT);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE,BLACK); // Draw white text
  display.setCursor(5, 0);
  // Display static text
  display.println("Program Monitor LED");
  display.setCursor(0, 8);
  display.println("---------------------");
  display.setCursor(0, 45);
  display.println("Powered By");
  display.setCursor(0, 55);
  display.println("Mr.Pornchai Thong-in");
  display.display(); 
}


void onled(){
  digitalWrite(led1, HIGH);
  display.setTextSize(1);
  display.setTextColor(WHITE,BLACK); // Draw white text
  display.setCursor(0, 21);
  display.println("LED1 = ON ");
  display.drawCircle(80, 21, 5, BLACK);
  display.fillCircle(80, 21, 5, WHITE);
  display.display();
}

void offled(){
  digitalWrite(led1, LOW); 
  display.setTextSize(1);
  display.setTextColor(WHITE,BLACK); // Draw white text
  display.setCursor(0, 21);
  display.println("LED1 = OFF");
  display.fillCircle(80, 21, 5, BLACK);
  display.drawCircle(80, 21, 5, WHITE);
  display.display();
}

void loop() {
  
  swState = digitalRead(sw1);
  if(swState == LOW){
      onled();
    }else{
    offled();
    }
}

