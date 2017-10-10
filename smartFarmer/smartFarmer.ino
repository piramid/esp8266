#include <AuthClient.h>
#include <MicroGear.h>
#include <MQTTClient.h>
#include <SHA1.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <EEPROM.h>
#include <MicroGear.h>
#include <string.h>

const char* ssid     = "WIFI_SSID";
const char* password = "WIFI_KEY";

#define APPID       "APPID"
#define GEARKEY     "APPKEY"
#define GEARSECRET  "APPSECRET"
#define SCOPE       ""


WiFiClient client;
AuthClient *authclient;

int timer = 0;
MicroGear microgear(client);

byte sensorInterrupt = 5;  // 0 = digital pin 2
byte sensorPin       = D1;


int sensorValue ;

// The hall-effect flow sensor outputs approximately 4.5 pulses per second per
// litre/minute of flow.
float calibrationFactor = 4.5;

volatile byte pulseCount;  

float flowRate;
unsigned int flowMilliLitres;
unsigned long totalMilliLitres;
unsigned long oldTime;

char Mode[128]="A" ;
char Order[128]="F" ;
int  X=30 ;
int  Y=500 ;

void onMsghandler(char *topic, uint8_t* msg, unsigned int msglen) {
  Serial.print("Incoming message --> ");
   digitalWrite(2,1);
   delay(100);
   digitalWrite(2,0);
   delay(100);
   digitalWrite(2,1);
   delay(100);
   digitalWrite(2,0);
   delay(100);
  //สัญญาณไฟ
  
 


msg[msglen] = '\0';
if (msg[0] == 'S'){
    msg += 1;
    X=my_str2int((char *) msg);  
    Serial.println(X);

    
    }
if (msg[0] == 'Q'){
    msg += 1;
   
    Y=my_str2int((char *) msg);
    Serial.println(Y);
  
    }
    

if (strcmp((char *)msg,"A")==0||strcmp((char *)msg,"M")==0){
  strcpy(Mode,(char *)msg);}
  else{strcpy(Order,(char *)msg);
    }
      
  Serial.println((char *)msg);

}
int my_str2int(const char *s)
{
    int res = 0;
    while (*s) {
        res *= 10;
        res += *s++ - '0';
    }

    return res;
}
void onFoundgear(char *attribute, uint8_t* msg, unsigned int msglen) {
  Serial.print("Found new member --> ");
  for (int i=0; i<msglen; i++)
    Serial.print((char)msg[i]);
  Serial.println();  
}

void onLostgear(char *attribute, uint8_t* msg, unsigned int msglen) {
  Serial.print("Lost member --> ");
  for (int i=0; i<msglen; i++)
    Serial.print((char)msg[i]);
  Serial.println();
}

void onConnected(char *attribute, uint8_t* msg, unsigned int msglen) {
  Serial.println("Connected to NETPIE...");
  microgear.setName("Autowater_Arduino");
}

void pulseCounter()
{
  // Increment the pulse counter
  pulseCount++;
}
void setup() {
    /* Event listener */
    microgear.on(MESSAGE,onMsghandler);
    microgear.on(PRESENT,onFoundgear);
    microgear.on(ABSENT,onLostgear);
    microgear.on(CONNECTED,onConnected);

    Serial.begin(115200);
    pinMode(16,OUTPUT);
    pinMode(4, OUTPUT);
    pinMode(0, OUTPUT);
    pinMode(2, OUTPUT);
    pinMode(sensorPin, INPUT);
    digitalWrite(sensorPin, HIGH);
    
    Serial.println("Starting...");

    if (WiFi.begin(ssid, password)) {

      while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
      }

      Serial.println("WiFi connected");  
      Serial.println("IP address: ");
      Serial.println(WiFi.localIP());

    //uncomment the line below if you want to reset token -->
      //microgear.resetToken();
      microgear.init(GEARKEY,GEARSECRET,SCOPE);
      microgear.connect(APPID);
    }
  pulseCount        = 0;
  flowRate          = 0.0;
  flowMilliLitres   = 0;
  totalMilliLitres  = 0;
  oldTime           = 0;

  sensorValue = analogRead(A0);
  attachInterrupt(sensorInterrupt, pulseCounter, FALLING);
}

void loop() {
 sensorValue = analogRead(A0);
int sensorValue2 = (100-(sensorValue)*100.0/1024.0);
 
    if (microgear.connected()) {
    Serial.println("connected");
    microgear.loop();
   
        Serial.println("Publish...");
 
      if (strcmp(Mode,"A")==0){
   Serial.print("Analog:  ");
   Serial.println(sensorValue);
   Serial.print("Moisture of Soil: ");
   Serial.print(sensorValue2);
   Serial.println(" %");

   
   if (sensorValue2<X)
      {
        digitalWrite(16,LOW);
        digitalWrite(4,1);
        digitalWrite(0,0);
        Serial.println("Status: On\t");
        microgear.chat("Autowater_HTML5","Status= ON");    
      }
   else 
      { 
        digitalWrite(16,HIGH);
        digitalWrite(4,0);
        digitalWrite(0,1);
        Serial.println("Status: Off\t");
        microgear.chat("Autowater_HTML5","Status= OFF");
      }

   
   if( totalMilliLitres >=Y)
      {
        digitalWrite(16,HIGH);
        digitalWrite(4,0);
        digitalWrite(0,1);
        Serial.println("Status: Off\t");
        microgear.chat("Autowater_HTML5","Status= OFF");
        
        
        totalMilliLitres=0;
        delay(20000);
      }
      
   if((millis() - oldTime) > 1000)    
      { 
    
        detachInterrupt(sensorInterrupt);
        flowRate = ((1000.0 / (millis() - oldTime)) * pulseCount) / calibrationFactor;
        oldTime = millis();
        flowMilliLitres = (flowRate / 60) * 1000; 
        totalMilliLitres += flowMilliLitres;


    unsigned int frac;
   
    // Determine the fractional part. The 10 multiplier gives us 1 decimal place.
    frac = (flowRate - int(flowRate)) * 10;

    // Print the number of litres flowed in this second
    Serial.print("Current Liquid Flowing: ");             // Output separator
    Serial.print(flowMilliLitres);
    Serial.println("mL/Sec");

    // Print the cumulative total of litres flowed since starting
    Serial.print("Output Liquid Quantity: ");             // Output separator
    Serial.print(totalMilliLitres);
    Serial.println("mL"); 
    Serial.println(""); 
    // Reset the pulse counter so we can start incrementing again
    pulseCount = 0;
    
    // Enable the interrupt again now that we've finished sending output
    attachInterrupt(sensorInterrupt, pulseCounter, FALLING);
    }
    delay(1000);}

    else {
          if (strcmp(Order,"N")==0){
            digitalWrite(16,LOW);
            digitalWrite(4,1);
            digitalWrite(0,0);
            Serial.println("Status: On\t");
            microgear.chat("Autowater_HTML5","Status= ON"); }
           else{ 
            digitalWrite(16,HIGH);
            digitalWrite(4,0);
            digitalWrite(0,1);
            Serial.println("Status: Off\t");
            microgear.chat("Autowater_HTML5","Status= OFF");    
              }
          
        
      unsigned int frac;
   
    // Determine the fractional part. The 10 multiplier gives us 1 decimal place.
    frac = (flowRate - int(flowRate)) * 10;detachInterrupt(sensorInterrupt);
        flowRate = ((1000.0 / (millis() - oldTime)) * pulseCount) / calibrationFactor;
        oldTime = millis();
        flowMilliLitres = (flowRate / 60) * 1000; 
       
        pulseCount = 0;
        attachInterrupt(sensorInterrupt, pulseCounter, FALLING);
        totalMilliLitres += flowMilliLitres;

         char msg[128];
         char msg1[128];
         char msg2[128];
                          sprintf(msg, "%d,", (int)flowMilliLitres);
                          sprintf(msg1, "%d,", (int)totalMilliLitres);
                          sprintf(msg2, "%d", (int)sensorValue2);
                          strcat(msg,msg1);
                          strcat(msg,msg2);
                          microgear.chat("Autowater_HTML5", msg);
                          microgear.chat("Autowater_HTML5", "Connected");
                          delay(1000);} 
  }
  
  else {
      Serial.println("connection lost, reconnect...");
      if (timer >= 5000) {
      microgear.connect(APPID);
      timer = 0;
    }
    else timer += 100;
  }
  
}

