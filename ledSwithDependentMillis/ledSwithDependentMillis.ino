/*example 1

1. ให้ไฟสีแดงกระพริบติด 3 วิ ดับ 3 วิ ตลอดเวลา
2. เมื่อกดปุ่ม ให้ไฟสีเขียวติดทันที
3. เมื่อปล่อยปุ่ม ให้ไฟสีเขียวดับ */

/***** Write by Mr. Pornchai Thong-in. Office Mahasarakham Technical College ******/

int ledState = LOW;             // ledState used to set the LED
unsigned long previousMillis = 0;        // will store last time LED was updated
const long interval = 3000;           // interval at which to blink (milliseconds)

const int BUTTON = 4;
const int RED = 15;
const int GREEN = 12;
//const int BLUE = 13;

int stateButton;

void setup() {
  // set the digital pin as output:
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  // pinMode(BLUE, OUTPUT);
  // set the digital pin as output:
  pinMode(BUTTON, INPUT);
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
   previousMillis = currentMillis; // save the last time you blinked the LED

    // if the LED is off turn it on and vice-versa:
    
//    if (ledState == LOW) {
//      ledState = HIGH;
//    } else {
//      ledState = LOW;
//    }
    ledState =! ledState;
    digitalWrite(RED, ledState);  // set the LED with the ledState of the variable:
  }
   
   // Check switch pressed or not 
  stateButton = digitalRead(BUTTON);
  if(stateButton == 0){
    digitalWrite(GREEN, HIGH);
    }
    else{
    digitalWrite(GREEN, LOW);
      }
}
