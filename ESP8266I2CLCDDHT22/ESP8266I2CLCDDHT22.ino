#include <LiquidCrystal_I2C.h>
#include <DHT.h>

#define DHTPIN 2     // what digital pin we're connected to D4
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321  
DHT dht(DHTPIN, DHTTYPE);

  LiquidCrystal_I2C lcd(0x3F,16,2); // Check I2C address of LCD, normally 0x27 or 0x3F
  
  uint8_t heart[8] = {0x0,0xa,0x1f,0x1f,0xe,0x4,0x0};  // example sprite bitmap
  uint8_t celcius[8] = {0x3,0x3,0x0,0x0,0x0,0x0,0x0};
  
  void setup()  {
    Serial.begin(115200);
    lcd.begin(D5,D6);      // In ESP8266-01, SDA=D5, SCL=D6               
    lcd.backlight();
    lcd.createChar(1, heart);
    lcd.createChar(0, celcius);
    lcd.home();                // At column=0, row=0
    lcd.print("Smart Farm IOT");   
    lcd.setCursor(0, 1);
    lcd.print("By Technocom MTC");
    lcd.setCursor(12, 0);      // At column=12, row=0
    lcd.write(byte(1));
    //lcd.setCursor(12, 0);      // At column=12, row=0
    //lcd.print(" ");            // Wipe sprite
    delay(5000);
    lcd.clear();
  }
  
  void loop()  {
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius
  float t = dht.readTemperature();
  
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    lcd.home();
    lcd.print("Failed to read");
    lcd.setCursor(0, 1);
    lcd.print("from DHT sensor");
    return;
  }

  // show in serial monitor
  Serial.print("Humidity: "); 
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: "); 
  Serial.print(t);
  Serial.print(" *C \n");
  lcd.home();                // At column=0, row=0
  lcd.print("Temp : "); 
  lcd.print(t); 
  lcd.write(byte(0)); //symbol celcius
  lcd.print("C");
  lcd.setCursor(0, 1);
  lcd.print("Humid: "); 
  lcd.print(h);
  lcd.print(" %");
  }
