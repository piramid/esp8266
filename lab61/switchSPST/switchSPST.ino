int sw1 = D3;
int led1 = D1;
int led2 = D2;

int swState = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(sw1, INPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  swState = digitalRead(sw1);
  if(swState == LOW){
    digitalWrite(led1,HIGH);
    }else{
      digitalWrite(led1, LOW);  
    }

}
