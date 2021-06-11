/*
    Ticker ESP8266
    Hardware: NodeMCU
    Circuits4you.com
    2018
    LED Blinking using Ticker
*/
#include <Ticker.h>  //Ticker Library

Ticker blinker;

int LED1 = D0;  //On board LED
int LED2 = D4;  //On board LED

void changeState()
{
  digitalWrite(LED1, !(digitalRead(LED1)));  //Invert Current State of LED  
}

void setup()
{
    Serial.begin(115200);
    Serial.println("");

    pinMode(LED1,OUTPUT);
    pinMode(LED2,OUTPUT);

    //Initialize Ticker every 0.5s
    blinker.attach(0.5, changeState); //Use attach_ms if you need time in ms
}

void loop()
{
 digitalWrite(LED2, HIGH);
 delay(100);
 digitalWrite(LED2, LOW);
 delay(100);
}
