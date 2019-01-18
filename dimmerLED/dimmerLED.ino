int led = D5;
int analog = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(led,OUTPUT);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  analog = analogRead(A0);
  Serial.print("analog :");
  Serial.println(analog);
  analogWrite(led,analog);
  delay(100);
  }
