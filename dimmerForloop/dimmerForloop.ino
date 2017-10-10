const int RED = 15;
const int GREEN = 12;
const int BLUE = 13;
long i;

void setup() { 
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
}

void loop() {
  for(i=0; i<=1023; i++){
  analogWrite(RED, i); 
  delay(1);
  }
  for(i=1023; i>=0; i--){
  analogWrite(RED, i); 
  delay(1);
  }
}
