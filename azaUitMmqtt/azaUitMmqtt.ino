 #include <OneWire.h>
#include <DallasTemperature.h>
#include <SPI.h>
#include <Wire.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//-----associte pin number with sensor/actuator 

//----ISTIHAR GPIO PIN ---------------------------------------------------------------------------------

//---=on board------
#define oneWireBus 15  //--SENSOR SUHU DS18B20
#define Relay01 27   //relay 1
#define Relay02 26   // relay 2
#define buzzer  25     // Buzzer
//--- device/sensor OFF board ------
#define varSensor  36
#define ldrSensor  34

#define SENSOR_PIN  15 // ESP32 pin GIOP21 connected to DS18B20 sensor's DQ pin
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
//----------------------------------- end -------------------------------------------------------------


// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

//----ISTIHAR SENSOR SUHU DS18B20 ---------------------------------------------------------------------
 
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);
//----------------------------------- end -------------------------------------------------------------


//---- PENETAPAN WIFI ---------------------------------------------------------------------------------

//------WIFI KKSBJ---
const char* ssid = "GUEST";  
const char* password = "";


WiFiClient espClient;                                                                          
PubSubClient client(espClient); //MQTT
const char* mqtt_server = "broker.hivemq.com";

//----------------------------------- end -------------------------------------------------------------

//---- PEMBOLEHUBAH DATA---------------------------------------------------------------------------------

char varPotString[8];

long lastMsg = 0;
char msg[50];
int value = 0;
//----------------------------------- end -------------------------------------------------------------



void setup() {

  pinMode (Relay01,OUTPUT);
  pinMode (Relay02,OUTPUT);
  pinMode (buzzer,OUTPUT);

Serial.begin(115200);

    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  
  delay(2000);
  display.clearDisplay();
  
  sensors.begin();
    
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  delay(100);  

 display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
   display.setCursor(0,0);
  display.println("IoT Socket Control");
//  display.setCursor(40, 20);
  display.display();
  delay(500);
    
}

void loop() {
if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();

  if (now - lastMsg > 2000) {  
    lastMsg = now;

//--------------------------------------------------
//-------------------Sensor Var --------------------------
     float datasensorVar = analogRead(varSensor);
     Serial.println(datasensorVar);
    
     dtostrf(datasensorVar, 1, 2, varPotString);
     client.publish("aza/var", varPotString);

  
//================ END ==============

 }
}

 