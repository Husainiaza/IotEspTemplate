#include <OneWire.h>
#include <DallasTemperature.h>
#include <WiFi.h>
#include <PubSubClient.h>
//------------oled display --------------
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

//---- on board GPIO ---------
#define oneWireBus 15
#define buzzer 25
#define relay1 26
#define relay1 27
//---- off board GPIO -------
#define sensorTanah  34

//----ISTIHAR SENSOR SUHU DS18B20 ---------------------------------------------------------------------
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);
//----------------------------------- end -------------------------------------------------------------

//----------- wifi and Broker -------------------------
const char* ssid = "GUEST";
const char* password = "";

//----------------------------------
//const char* ssid = "Feelur";
//const char* password = "nisa12345";

const char* mqtt_server = "broker.hivemq.com";

WiFiClient espClient;
PubSubClient client(espClient);

//-------- Data variable
long lastMsg = 0;
char msg[50];
int value = 0;
String AzaText;
char dataPotString[8];
float temperatureC;

//---- pembolehubah memori data tersimpan dari bacaan sensor ------
int dataSensorTanah;

void setup() {
pinMode(relay1,OUTPUT); 
pinMode(buzzer,OUTPUT); 

Serial.begin(115200);
 
lcd.init();                      // initialize the lcd 
  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(3,0);
  lcd.print("Hello, world!");
  lcd.setCursor(2,1);
  lcd.print("Ywrobot Arduino!");
  lcd.setCursor(0,2);
  lcd.print("Arduino LCM IIC 2004");
  lcd.setCursor(2,3);
  lcd.print("Power By Ec-yuan!");

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  sensors.begin();
    
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

 long now = millis();
  
  if (now - lastMsg > 2000) {  
      lastMsg = now;

//--koding sensor anda bermula disini --------

//----sensor suhu-----------------------------------
    sensors.requestTemperatures(); 
    temperatureC = sensors.getTempCByIndex(0);
    dtostrf(temperatureC, 1, 2, dataPotString);
    Serial.print("suhu : ");
    Serial.println(temperatureC);
    client.publish("psp/sensorSuhu", dataPotString);
     //---- sensor 1

//----sensor tanah-----------------------------------
    dataSensorTanah = analogRead(sensorTanah);  

     if (dataSensorTanah >2000) {
       Serial.println("beep");
       digitalWrite(buzzer,HIGH);
    }
      else
    {
      digitalWrite(buzzer,LOW);
    }

    dtostrf(dataSensorTanah, 1, 2, dataPotString);
    Serial.print("kelembapab Tanah : ");
    Serial.println(dataSensorTanah);
    client.publish("psp/sensorTanah", dataPotString);
 
  
 //-- koding sensor anda berakahir ----------
/*
    display.clearDisplay();
    display.setCursor(0,0);
    display.println("Plant Monitoring");
    display.setTextSize(1);
    display.setTextColor(WHITE,BLACK);
    display.setCursor(0,20);
    display.print("SUHU:");
    display.print(temperatureC,1);
    display.setCursor(0,30);
    display.print("Tanah :");
    display.print(dataSensorTanah);
    display.display(); */
      
  }
}
