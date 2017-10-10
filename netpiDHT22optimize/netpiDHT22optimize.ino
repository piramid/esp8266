/*  NETPIE ESP8266 basic sample                            */
/*  More information visit : https://netpie.io             */

#include <ESP8266WiFi.h>
#include <MicroGear.h>
#include <DHT.h>

const char* ssid     = "guitar";
const char* password = "aaaaabbbbb";

#define APPID    "iotvec"
#define KEY      "g3UFP03pUwWYq7l"
#define SECRET   "ELnEOZEg9Dn1jfRCaUQem5m2y"
#define ALIAS   "tomDevice"
#define FEEDID "tomsmartfeed"
int ledPin1 = 12;
int ledPin2 = 14;
int ledPin3 = 16;
int ledPin4 = 2;
int pump = 13;

unsigned long times=0;
unsigned long reconn=0;


#define DHTPIN 6     // what digital pin we're connected to
#define DHTTYPE DHT11   // DHT 22  (AM2302), AM2321  
DHT dht(DHTPIN, DHTTYPE);

WiFiClient client;

int timer = 0;
char str[32];
MicroGear microgear(client);

/* If a new message arrives, do this */
void onMsghandler(char *topic, uint8_t* msg, unsigned int msglen) {
    Serial.print("Incoming message --> ");
      msg[msglen] = '\0';

      char strState[msglen];
      for (int i = 0; i < msglen; i++) 
      {
        strState[i] = (char)msg[i];
        Serial.print((char)msg[i]);
      }
     
      Serial.println();
    
      String stateStr = String(strState).substring(0, msglen);

    if (stateStr == "SW1ON") 
      {
       Serial.println("SW1 On");
         digitalWrite(ledPin1, LOW);
          microgear.chat("sw1", "ON"); 
      }      
      else if (stateStr == "SW1OFF") 
      {
        digitalWrite(ledPin1, HIGH);
        microgear.chat("sw1", "OFF");       
      }

     if (stateStr == "SW2ON") 
      {
       Serial.println("SW2 On");
         digitalWrite(ledPin2, LOW);
          microgear.chat("sw2", "ON"); 
      }      
      else if (stateStr == "SW2OFF") 
      {
        digitalWrite(ledPin2, HIGH);
        microgear.chat("sw2", "OFF");       
      }

     if (stateStr == "SW3ON") 
      {
       Serial.println("SW3 On");
         digitalWrite(ledPin3, LOW);
          microgear.chat("sw3", "ON"); 
      }      
      else if (stateStr == "SW3OFF") 
      {
        digitalWrite(ledPin3, HIGH);
        microgear.chat("sw3", "OFF");       
      }

     if (stateStr == "SW4ON") 
      {
       Serial.println("SW4 On");
         digitalWrite(ledPin4, LOW);
          microgear.chat("sw4", "ON"); 
      }      
      else if (stateStr == "SW4OFF") 
      {
        digitalWrite(ledPin4, LOW);
        microgear.chat("sw4", "OFF");       
      }

     if (stateStr == "pumpON") 
      {
       Serial.println("pump On");
         digitalWrite(pump, LOW);
          microgear.chat("pump", "ON"); 
      }      
      else if (stateStr == "pumpOFF") 
      {
        digitalWrite(pump, HIGH);
        microgear.chat("pump", "OFF");       
      }
      
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

/* When a microgear is connected, do this */
void onConnected(char *attribute, uint8_t* msg, unsigned int msglen) {
    Serial.println("Connected to NETPIE...");
    /* Set the alias of this microgear ALIAS */
    microgear.setAlias(ALIAS);
}


void setup() {
    /* Add Event listeners */

    /* Call onMsghandler() when new message arraives */
    microgear.on(MESSAGE,onMsghandler);

    /* Call onFoundgear() when new gear appear */
    microgear.on(PRESENT,onFoundgear);

    /* Call onLostgear() when some gear goes offline */
    microgear.on(ABSENT,onLostgear);

    /* Call onConnected() when NETPIE connection is established */
    microgear.on(CONNECTED,onConnected);

    Serial.begin(115200);
    Serial.println("Starting...");

    /* Initial WIFI, this is just a basic method to configure WIFI on ESP8266.                       */
    /* You may want to use other method that is more complicated, but provide better user experience */
    if (WiFi.begin(ssid, password)) {
        while (WiFi.status() != WL_CONNECTED) {
            delay(500);
            Serial.print(".");
        }
    }

    Serial.println("WiFi connected");  
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    /* Initial with KEY, SECRET and also set the ALIAS here */
    microgear.init(KEY,SECRET,ALIAS);

    /* connect to NETPIE to a specific APPID */
    microgear.connect(APPID);

    pinMode(ledPin1, OUTPUT);
    digitalWrite(ledPin1, LOW);
    microgear.chat("sw1", "OFF"); 
    pinMode(ledPin2, OUTPUT);
    digitalWrite(ledPin2, LOW);
    microgear.chat("sw2", "OFF"); 
    pinMode(ledPin3, OUTPUT);
    digitalWrite(ledPin3, LOW);
    microgear.chat("sw3", "OFF"); 
    pinMode(ledPin4, OUTPUT);
    digitalWrite(ledPin4, LOW);
    microgear.chat("sw4", "OFF"); 
    digitalWrite(pump, LOW);
    microgear.chat("pump", "OFF"); 
}

void loop() {
    /* To check if the microgear is still connected */
    if (microgear.connected()) {
        Serial.println("connected");

        /* Call this method regularly otherwise the connection may be lost */
        microgear.loop();

         // if (timer >= 1000) {  
         if(millis()-times >=1000){
                times = millis();
                float h = dht.readHumidity();
                float t = dht.readTemperature();
                float f = dht.readTemperature(true);
                sprintf(str,"%d,%d",h,t);
                Serial.println(str);
                String data = "{\"humidity\":";
                       data += h;
                       data += ",\"temperature\":";
                       data += t;
                       data += "}";
                Serial.println((char*)data.c_str());
                delay(2000);
                    
                  if (isnan(h) || isnan(t) || isnan(f)) {
                    Serial.println("Failed to read from DHT sensor!");
                    return;
                  } 
                Serial.println("t");
                Serial.println(t);
                Serial.println("h");
                Serial.println(h);
                
                microgear.chat("t", t); 
                microgear.chat("h", h);
                microgear.writeFeed(FEEDID,data);
          } else timer += 100;

        
    }
    else {
        Serial.println("connection lost, reconnect...");
       // if (timer >= 5000) {
       if (millis()-reconn >= 5000){
            reconn = millis();
            microgear.connect(APPID);
            timer = 0;
        }
        else timer += 100;
    }
    delay(100);
}

