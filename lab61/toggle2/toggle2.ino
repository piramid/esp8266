int ledPin = 5;
int buttonPin = 4;
int reading = 0;
 
boolean buttonState;
boolean lastState;
boolean state = HIGH;
 
 void setup() {
  pinMode (buttonPin,INPUT);
  pinMode (ledPin,OUTPUT);
}
 
void loop() {
  reading = digitalRead(buttonPin);
 
  if ( reading == LOW && lastState == HIGH ) {
    delay (10);
    if (digitalRead(buttonPin) == LOW) state = !state;
  }
 
  digitalWrite(ledPin,state);
  lastState = reading;
 
}
