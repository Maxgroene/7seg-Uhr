#include <WiFi.h>
#include "Arduino.h"
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <ESP32Servo.h>
#include <PubSubClient.h>
#include "mqtt.cpp"
#include "servo.cpp"
#include "display.h"
#include "led.h"

const int numServos = 4; 
Servo servo[numServos];  
const int servoPins[numServos] = {32, 33, 25, 26};
int servoPos[4][10] = {
  {178, 140, 125, 100, 85, 60, 45, 24, 8, 0},
  {178, 140, 125, 100, 85, 60, 45, 24, 8, 0},
  {178, 140, 125, 100, 85, 60, 45, 24, 8, 0},
  {178, 140, 125, 100, 85, 60, 45, 24, 8, 0},
};

#define DHTPIN 4
#define DHTTYPE DHT22 

const int ledGreen = 15; 
const int ledRed = 2; 

const int potPin = 34;
int potValue = 0;

const int lcdColumns = 16;
const int lcdRows = 2;
const int lcdAddress = 0x27;

const char* ssid = "BKR-HotSpot";
const char* password = "";
const char* mqtt_server = "mqtt.mgroe.de";
const char* mqtt_user = "max";
const char* mqtt_password = "Hallo.123";
const char* mqtt_temperature_topic = "esp01/temperature";
const char* mqtt_humidity_topic = "esp01/humidity";

ServoManager servoManager;
MQTTManager mqttManager;
WiFiClient espClient;
PubSubClient client(espClient);
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(lcdAddress, lcdColumns, lcdRows);

int test = 0;

void setup() {
  pinMode(ledGreen, OUTPUT);
  pinMode(ledRed, OUTPUT);

  lcd.init();
  lcd.backlight();

  WiFi.begin(ssid, password);
  Serial.print("Verbindung mit ");
  Serial.println(ssid);

  mqttManager.setup();
  servoManager.setup();

  servoManager.SetNumber(0000);
}

void loop() {
  if (WiFi.status() != WL_CONNECTED){
    WiFi.reconnect();
  }
  mqttManager.mqttLoop();
  
  servoManager.SetNumber(test);
  test += 1;
  
  delay(300);
}
