#define LED_RED D2
#define LED_GREEN D1
#define BUTTON D3
void setup() {
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(BUTTON, INPUT);
  attachInterrupt(BUTTON, showGreen, CHANGE);
}
void loop() {
  digitalWrite(LED_RED, HIGH);
  delay(3000);
  digitalWrite(LED_RED, LOW);
  delay(3000);
}
void showGreen() {
  digitalWrite(LED_GREEN, !digitalRead(BUTTON));
}
