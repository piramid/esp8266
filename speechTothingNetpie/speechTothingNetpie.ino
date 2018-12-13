/*  NETPIE ESP8266 basic sample                            */
/*  More information visit : https://netpie.io             */

#include <ESP8266WiFi.h>
#include <MicroGear.h>
#include "DHT.h"

const char* ssid     = " ";
const char* password = " ";

#define APPID   "aaaa"
#define KEY     "aaaa"
#define SECRET  "aaaa"
#define ALIAS   "speech"

#define LEDSTATETOPIC "/light/" ALIAS      // topic ที่ต้องการ publish ส่งสถานะ led ในที่นี้จะเป็น /ledstate/{ชื่อ alias ตัวเอง}
#define DHTDATATOPIC "/dht/" ALIAS            // topic ที่ต้องการ publish ส่งข้อมูล dht ในที่นี่จะเป็น /dht/{ชื่อ alias ตัวเอง}
#define TEMPTOPIC "/dhttemp/"            // topic ที่ต้องการ publish ส่งข้อมูล dhttemp ในที่นี่จะเป็น /dhttemp/
#define HUMIDTOPIC "/dhthumid/"

#define BUTTONPIN  D3                         // pin ที่ต่อกับปุ่ม Flash บนบอร์ด NodeMCU
#define LEDPIN     LED_BUILTIN                // pin ที่ต่อกับไฟ LED บนบอร์ด NodeMCU

int currentLEDState = 0;      // ให้เริ่มต้นเป็น OFF
int lastLEDState = 1;
int currentButtonState = 1;   // หมายเหตุ ปุ่ม flash ต่อเข้ากับ GPIO0 แบบ pull-up
int lastButtonState = 0;


#define DHTPIN    D4          // GPIO2 ขาที่ต่อเข้ากับขา DATA (บางโมดูลใช้คำว่า OUT) ของ DHT
#define DHTTYPE   DHT22       // e.g. DHT11, DHT21, DHT22
DHT dht(DHTPIN, DHTTYPE);

float humid = 0;     // ค่าความชื้น
float temp  = 0;     // ค่าอุณหภูมิ

long lastDHTRead = 0;
long lastDHTPublish = 0;

long lastTimeFeed = 0;

WiFiClient client;

int timer = 0;
MicroGear microgear(client);

/* If a new message arrives, do this */
void onMsghandler(char *topic, uint8_t* msg, unsigned int msglen) {
    Serial.print("Incoming message --> ");
    msg[msglen] = '\0';

    Serial.println((char *)msg);

   String msg2 = String((char*)msg);
    if (msg2 == "led1on") {
      digitalWrite(D5, LOW);
      microgear.chat("sw1", "ON");
      }
      else if (msg2 == "led1off") {
      digitalWrite(D5, HIGH);
      microgear.chat("sw1", "OFF");
      }
    if (msg2 == "led2on") {
      digitalWrite(D6, LOW);
      microgear.chat("sw2", "ON");
      }
      else if (msg2 == "led2off") {
      digitalWrite(D6, HIGH);
      microgear.chat("sw2", "OFF");
      }
    if (msg2 == "led3on") {
      digitalWrite(D7, LOW);
      microgear.chat("sw3", "ON");
      }
      else if (msg2 == "led3off") {
      digitalWrite(D7, HIGH);
      microgear.chat("sw3", "OFF");
      }
     if (msg2 == "led4on") {
      digitalWrite(D8, LOW);
      microgear.chat("sw4", "ON");
      }
      else if (msg2 == "led4off") {
      digitalWrite(D8, HIGH);
      microgear.chat("sw4", "OFF");
    }
    if (msg2 == "ledon") {
      digitalWrite(D5, LOW);
      digitalWrite(D6, LOW);
      digitalWrite(D7, LOW);
      digitalWrite(D8, LOW);
      microgear.chat("sw1", "ON");
      microgear.chat("sw2", "ON");
      microgear.chat("sw3", "ON");
      microgear.chat("sw4", "ON");
      }
    if (msg2 == "ledoff") {
      digitalWrite(D5, HIGH);
      digitalWrite(D6, HIGH);
      digitalWrite(D7, HIGH);
      digitalWrite(D8, HIGH);
      microgear.chat("sw1", "OFF");
      microgear.chat("sw2", "OFF");
      microgear.chat("sw3", "OFF");
      microgear.chat("sw4", "OFF");
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
  
    pinMode(D5,OUTPUT);
    pinMode(D6,OUTPUT);
    pinMode(D7,OUTPUT);
    pinMode(D8,OUTPUT);
    digitalWrite(D5, HIGH);
    digitalWrite(D6, HIGH);
    digitalWrite(D7, HIGH);
    digitalWrite(D8, HIGH);
    
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
}

void loop() {
    /* To check if the microgear is still connected */
    if (microgear.connected()) {
        Serial.println("connected");

        /* Call this method regularly otherwise the connection may be lost */
        microgear.loop();

        if (timer >= 1000) {
            Serial.println("Publish...");

            /* Chat with the microgear named ALIAS which is myself */
            microgear.chat(ALIAS,"Hello");
            timer = 0;

            microgear.subscribe("/light");       
        } 
        else timer += 100;
        if(millis() - lastDHTRead > 2000){
          humid = dht.readHumidity();     // อ่านค่าความชื้น
          temp  = dht.readTemperature();  // อ่านค่าอุณหภูมิ
          lastDHTRead = millis();
          
          Serial.print("Humid: "); Serial.print(humid); Serial.print(" %, ");
          Serial.print("Temp: "); Serial.print(temp); Serial.println(" °C ");
    
          // ตรวจสอบค่า humid และ temp เป็นตัวเลขหรือไม่
          if (isnan(humid) || isnan(temp)) {
            Serial.println("Failed to read from DHT sensor!");
          }
          else{
            // เตรียมสตริงในรูปแบบ "{humid},{temp}"
            String datastring = (String)humid+","+(String)temp;
            Serial.print("Sending --> ");
            Serial.println(datastring);
            //microgear.publish(DHTDATATOPIC,datastring);   // DHTDATATOPIC ถูก define ไว้ข้างบน
            microgear.publish(DHTDATATOPIC,datastring,1); // เรียกดูค่าสุดท้าย
            delay(3500);
            microgear.publish(TEMPTOPIC,temp,1); //นำค่าที่ได้ไปแสดงใน speech to thing
            microgear.publish(HUMIDTOPIC,humid,1);
          }
        }  
    }
    else {
        Serial.println("connection lost, reconnect...");
        if (timer >= 5000) {
            microgear.connect(APPID);
            timer = 0;
        }
        else timer += 100;
    }
    delay(100);
}
