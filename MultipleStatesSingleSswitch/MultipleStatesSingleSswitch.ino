#include <Wire.h>
//#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 OLED(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int pin_LEDgreen = D5;
int pin_LEDyellow = D6;
int pin_LEDred = D7;
 
int pin_switch = D4;
 
// variables to hold the new and old switch states
boolean oldSwitchState = LOW;
boolean newSwitchState1 = LOW;
boolean newSwitchState2 = LOW;
boolean newSwitchState3 = LOW;
 
byte state = 0;
 
void setup() 
{
    Serial.begin(9600);
    Serial.print("Sketch:   ");   
    Serial.println(__FILE__);
    Serial.print("Uploaded: ");   
    Serial.println(__DATE__);
    Serial.println(" ");
    
    OLED.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    OLED.clearDisplay();
    OLED.setTextColor(WHITE);
    OLED.setCursor(0,0);
    OLED.setTextSize(2);
    OLED.println("My Program");
    OLED.display();
 
    pinMode(pin_LEDgreen, OUTPUT);    
    digitalWrite(pin_LEDgreen,LOW); 
    pinMode(pin_LEDyellow, OUTPUT);   
    digitalWrite(pin_LEDyellow,LOW); 
    pinMode(pin_LEDred, OUTPUT);      
    digitalWrite(pin_LEDred,LOW); 
 
    pinMode(pin_switch, INPUT); 
}
 
void loop()
{
    newSwitchState1 = digitalRead(pin_switch);
    delay(1);
    newSwitchState2 = digitalRead(pin_switch);
    delay(1);
    newSwitchState3 = digitalRead(pin_switch);
 
    // if all 3 values are the same we can continue
    if (  (newSwitchState1==newSwitchState2) && (newSwitchState1==newSwitchState3) )
    {
 
        if ( newSwitchState1 != oldSwitchState ) 
        {
 
           // has the button switch been closed?
           if ( newSwitchState1 == HIGH )
           {
                // increase the value of state
                state++;
                if (state > 3) { state = 0; }
 
                // turn all LEDs off. Doing it this way means we do not need to care about the individual LEDs
                // simply turn them all off and then turn on the correct one. 
                digitalWrite(pin_LEDgreen, LOW);
                digitalWrite(pin_LEDyellow, LOW);
                digitalWrite(pin_LEDred, LOW);
                OLED.clearDisplay();
                OLED.setTextColor(WHITE);
                OLED.setCursor(0,10);
                OLED.setTextSize(2);
                OLED.println("LED1 OFF ");
                OLED.println("LED2 OFF ");
                OLED.println("LED3 OFF ");
                OLED.display();
                Serial.println("LED1 OFF");
                Serial.println("LED2 OFF");
                Serial.println("LED3 OFF");
 
                // Turn on the next LED
                // Because the value of state does not change while we are testing it we don't need to use else if
                if (state==1) { digitalWrite(pin_LEDgreen, HIGH);
                  OLED.clearDisplay();
                  OLED.setTextColor(WHITE);
                  OLED.setCursor(0,10);
                  OLED.setTextSize(2);
                  OLED.println("LED1 ON ");
                  OLED.println("LED2 OFF ");
                  OLED.println("LED3 OFF ");
                  OLED.display();
                  Serial.println("LED1 ON");
                  Serial.println("LED2 OFF");
                  Serial.println("LED3 OFF");
                }
                if (state==2) { digitalWrite(pin_LEDyellow, HIGH);
                  OLED.clearDisplay();
                  OLED.setTextColor(WHITE);
                  OLED.setCursor(0,10);
                  OLED.setTextSize(2);
                  OLED.println("LED1 OFF ");
                  OLED.println("LED2 ON ");
                  OLED.println("LED3 OFF ");
                  OLED.display();
                  Serial.println("LED1 OFF");
                  Serial.println("LED2 ON");
                  Serial.println("LED3 OFF");
                }
                if (state==3) { digitalWrite(pin_LEDred, HIGH);  
                  OLED.clearDisplay();
                  OLED.setTextColor(WHITE);
                  OLED.setCursor(0,10);
                  OLED.setTextSize(2);
                  OLED.println("LED1 OFF ");
                  OLED.println("LED2 OFF ");
                  OLED.println("LED3 ON ");
                  OLED.display();
                  Serial.println("LED1 OFF");
                  Serial.println("LED2 OFF");
                  Serial.println("LED3 ON");
                }
 
           }
           oldSwitchState = newSwitchState1;
        }  
    }
}
