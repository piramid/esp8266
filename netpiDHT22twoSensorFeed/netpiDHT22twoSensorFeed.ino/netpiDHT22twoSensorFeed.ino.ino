#include <ESP8266WiFi.h>
#include <MicroGear.h>
#include "DHT.h"                              // library สำหรับอ่านค่า DHT Sensor ต้องติดตั้ง DHT sensor library by Adafruit v1.2.3 ก่อน

// ----- แก้ค่า config 7 ค่าข้างล่างนี้ --------------------------------------------------------
const char* ssid     = "tom";       // ชื่อ ssid
const char* password = "aaaaabbbb1";        // รหัสผ่าน wifi

#define APPID   "netpietrain"                       // ให้แทนที่ด้วย AppID รวม
#define KEY     "IgkZUQIzaxZtDdN"                         // ให้แทนที่ด้วย Key รวม
#define SECRET  "TzIbvfseY7Fg4DbiIzk0MRSVR"                      // ให้แทนที่ด้วย Secret รวม

#define ALIAS   "A01"           // แทนที่ด้วยหมายเลขของท่าน เช่น "A01"
#define NEIGHBOR "dth2sensor"             // ชื่ออุปกรณ์ของเพื่อน เช่น "A02"
// --------------------------------------------------------------------------------------

#define LEDSTATETOPIC "/ledstate/" ALIAS      // topic ที่ต้องการ publish ส่งสถานะ led ในที่นี้จะเป็น /ledstate/{ชื่อ alias ตัวเอง}
#define DHTDATATOPIC1 "/dht1/" ALIAS            // topic ที่ต้องการ publish ส่งข้อมูล dht ในที่นี่จะเป็น /dht/{ชื่อ alias ตัวเอง}
#define DHTDATATOPIC2 "/dht2/" ALIAS            // topic ที่ต้องการ publish ส่งข้อมูล dht ในที่นี่จะเป็น /dht/{ชื่อ alias ตัวเอง}

#define BUTTONPIN  D3                         // pin ที่ต่อกับปุ่ม Flash บนบอร์ด NodeMCU
#define LEDPIN     LED_BUILTIN                // pin ที่ต่อกับไฟ LED บนบอร์ด NodeMCU

#define FEEDID   "dth2sensorfeed"           // ให้แทนที่ด้วย FeedID
#define FEEDAPI  "WiKbXNoivaqRmWZnGmSKOHq5Sxg5PCl5"          // ให้แทนที่ด้วย FeedAPI

int currentLEDState = 0;      // ให้เริ่มต้นเป็น OFF
int lastLEDState = 1;
int currentButtonState = 1;   // หมายเหตุ ปุ่ม flash ต่อเข้ากับ GPIO0 แบบ pull-up
int lastButtonState = 0;

#define DHTPIN1    D4          // GPIO2 ขาที่ต่อเข้ากับขา DATA (บางโมดูลใช้คำว่า OUT) ของ DHT
#define DHTPIN2    D5
#define DHTTYPE   DHT22       // e.g. DHT11, DHT21, DHT22
DHT dht1(DHTPIN1, DHTTYPE);
DHT dht2(DHTPIN2, DHTTYPE);

float humid1 = 0;     // ค่าความชื้น
float temp1  = 0;     // ค่าอุณหภูมิ
float humid2 = 0;     // ค่าความชื้น
float temp2  = 0;     // ค่าอุณหภูมิ

long lastDHTRead = 0;
long lastDHTPublish = 0;

long lastTimeWriteFeed = 0;

WiFiClient client;
MicroGear microgear(client);

void updateLED(int state) {
    currentLEDState = state;

    // ไฟ LED บน NodeMCU เป็น active-low จะติดก็ต่อเมื่อส่งค่า LOW ไปให้ LEDPIN
    if (currentLEDState == 1) digitalWrite(LEDPIN, LOW); // LED ON
    else  digitalWrite(LEDPIN, HIGH); // LED OFF
}    

void onMsghandler(char *topic, uint8_t* msg, unsigned int msglen) {
    Serial.print("Incoming message --> ");
    msg[msglen] = '\0';
    Serial.println((char *)msg);

    if (*(char *)msg == '0') updateLED(0);
    else if (*(char *)msg == '1') updateLED(1);
}

void onConnected(char *attribute, uint8_t* msg, unsigned int msglen) {
    Serial.println("Connected to NETPIE...");
    microgear.setAlias(ALIAS);
}

void setup() {
    microgear.on(MESSAGE,onMsghandler);
    microgear.on(CONNECTED,onConnected);

    Serial.begin(115200);
    Serial.println("Starting...");
    dht1.begin(); // initialize โมดูล DHT
    dht2.begin(); // initialize โมดูล DHT

    // กำหนดชนิดของ PIN (ขาI/O) เช่น INPUT, OUTPUT เป็นต้น
    pinMode(LEDPIN, OUTPUT);          // LED pin mode กำหนดค่า
    pinMode(BUTTONPIN, INPUT);        // Button pin mode รับค่า
    updateLED(currentLEDState);

    if (WiFi.begin(ssid, password)) {
        while (WiFi.status() != WL_CONNECTED) {
            delay(1000);
            Serial.print(".");
        }
    }
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    microgear.init(KEY,SECRET,ALIAS);   // กำหนดค่าตันแปรเริ่มต้นให้กับ microgear
    microgear.connect(APPID);           // ฟังก์ชั่นสำหรับเชื่อมต่อ NETPIE
}

void loop() {
    if (microgear.connected()) {
        microgear.loop();

        if(currentLEDState != lastLEDState){
          microgear.publish(LEDSTATETOPIC, currentLEDState);  // LEDSTATETOPIC ถูก define ไว้ข้างบน
          lastLEDState = currentLEDState;
        }

        if (digitalRead(BUTTONPIN)==HIGH) currentButtonState = 0;
        else currentButtonState = 1;

        if(currentButtonState != lastButtonState){
          microgear.chat(NEIGHBOR, currentButtonState);
          lastButtonState = currentButtonState;
        }

        // เซนเซอร์​ DHT อ่านถี่เกินไปไม่ได้ จะให้ค่า error เลยต้องเช็คเวลาครั้งสุดท้ายที่อ่านค่า
        // ว่าทิ้งช่วงนานพอหรือยัง ในที่นี้ตั้งไว้ 2 วินาที ก
        if(millis() - lastDHTRead > 2000){
          humid1 = dht1.readHumidity();     // อ่านค่าความชื้น
          temp1  = dht1.readTemperature();  // อ่านค่าอุณหภูมิ
          humid2 = dht2.readHumidity();     // อ่านค่าความชื้น
          temp2  = dht2.readTemperature();  // อ่านค่าอุณหภูมิ
          lastDHTRead = millis();
          
          Serial.print("Humid: "); Serial.print(humid1); Serial.print(" %, ");
          Serial.print("Temp: "); Serial.print(temp1); Serial.println(" C ");
    
          // ตรวจสอบค่า humid และ temp เป็นตัวเลขหรือไม่
          if (isnan(humid1) || isnan(temp1)) {
            Serial.println("Failed to read from DHT sensor!");
          }
          else{
            // เตรียมสตริงในรูปแบบ "{humid},{temp}"
            String datastring1 = (String)humid1+","+(String)temp1;
            String datastring2 = (String)humid2+","+(String)temp2;
            Serial.print("Sending data 1 --> ");
            Serial.println(datastring1);
            Serial.print("Sending data 2--> ");
            Serial.println(datastring2);
            microgear.publish(DHTDATATOPIC1,datastring1);   // DHTDATATOPIC ถูก define ไว้ข้างบน
            microgear.publish(DHTDATATOPIC2,datastring2);   // DHTDATATOPIC ถูก define ไว้ข้างบน
          }
        }
        
        if(millis()-lastTimeWriteFeed > 15000){
          lastTimeWriteFeed = millis();
          if(humid1!=0 && temp1!=0){
            String data = "{\"humid\":";
            data += humid1 ;
            data += ", \"temp\":";
            data += temp1 ;
            data += ", \"humid2\":";
            data += humid2 ;
            data += ", \"temp2\":";
            data += temp2 ;
            data += "}"; 

            // field name
            // humid, temp, humid2, temp2
            // data
            // {"temp":25.44, "humid": 42.53}  
            // "temp":25.44, "humid": 42.53
            
            Serial.print("Write Feed --> ");
            Serial.println(data);
            microgear.writeFeed(FEEDID,data);
            //microgear.writeFeed(FEEDID,data,FEEDAPI);
          }
        }
    }
    else {
        Serial.println("connection lost, reconnect...");
        microgear.connect(APPID); 
    }
}
