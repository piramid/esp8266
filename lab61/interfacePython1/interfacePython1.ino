const int led = 12; // define led as the constant integer 12
// connect the positive end of the LED to pin 12 of the Arduino
char val; //define a variable val as a character

void setup(){
  Serial.begin(9600); // begin the serial communication with a baudrate of 9600
  pinMode(led, OUTPUT); //set led (pin 12) as a digital output
}

void loop(){
  if (Serial.available()){ //run only if there is serial communication available
    val = Serial.read(); //read the serial communication and set val as the read value
    if (val == 'A'){ // if val is equal to uppercase A
      digitalWrite(led, HIGH); // set the led pin to high, turn the led on
    }
    if (val == 'a'){ // if val is equal to lowercase a
      digitalWrite(led, LOW); // set the led pin to low, turn the led off
    }
  }
delay(100); //sleep for a tenth of a second before the loop begins again
}
