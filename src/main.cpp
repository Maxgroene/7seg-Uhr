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
#include "DCF77.h"
#include "TimeLib.h"

const int numServos = 4;
Servo servo[numServos];
const int servoPins[numServos] = {32, 33, 25, 26};
int servoPos[4][10] = {
    {178, 150, 130, 110, 90, 70, 50, 35, 20, 0},
    {178, 160, 130, 110, 90, 70, 55, 30, 17, 0},
    {178, 150, 130, 110, 90, 70, 55, 35, 17, 0},
    {178, 150, 130, 110, 90, 70, 50, 35, 20, 0},
};

#define DHTPIN 4
#define DHTTYPE DHT22

#define DCF_PIN 2           // Connection pin to DCF 77 device
#define DCF_INTERRUPT 0    // Interrupt number associated with pin
#define PIN_LED 13

const int ledGreen = 15;
const int ledRed = 2;

const int potPin = 34;
int potValue = 0;

const int lcdColumns = 16;
const int lcdRows = 2;
const int lcdAddress = 0x27;

const char *ssid = "BKR-HotSpot";
const char *password = "";
const char *mqtt_server = "mqtt.mgroe.de";
const char *mqtt_user = "max";
const char *mqtt_password = "Hallo.123";
const char *mqtt_temperature_topic = "esp01/temperature";
const char *mqtt_humidity_topic = "esp01/humidity";
const char *mqtt_time_topic = "esp01/uhr";

ServoManager servoManager;
MQTTManager mqttManager;
int MQTTManager::time;
WiFiClient espClient;
PubSubClient client(espClient);
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(lcdAddress, lcdColumns, lcdRows);
DCF77 DCF = DCF77(DCF_PIN, DCF_INTERRUPT);

bool g_bDCFTimeFound = false;

int test = 0;

void setup()
{
  Serial.begin(115200);
  pinMode(ledGreen, OUTPUT);
  pinMode(ledRed, OUTPUT);

  lcd.init();
  lcd.backlight();

  DCF.Start();
  Serial.println("Warten auf DCF77-Zeit... ");
  Serial.println("Dies dauert mindestens 2 Minuten, in der Regel eher länger.");  

  WiFi.begin(ssid, password);
  Serial.print("Verbindung mit ");
  Serial.println(ssid);
  lcd.print("Verbindung mit");
  lcd.setCursor(0, 1);
  lcd.print(ssid);

  mqttManager.setup();
  servoManager.setup();

  servoManager.SetNumber(0000);
}

void loop()
{

  potValue = analogRead(potPin);
  int tempPot = map(potValue, 0, 8190, 0, 5); //  or 4095

  if (WiFi.status() != WL_CONNECTED)
  {
    WiFi.reconnect();
  }

  mqttManager.mqttLoop();

  lcd.clear();

  switch (tempPot)
  {
  case 0:
    lcd.setCursor(0, 0);
    lcd.print("Hochzaehlen:");
    lcd.setCursor(0, 1);
    lcd.print(test);
    servoManager.SetNumber(test);
    test += 10;
    break;
  case 1:
    lcd.setCursor(0, 0);
    lcd.print("Temperatur:");
    lcd.setCursor(0, 1);
    lcd.print(mqttManager.temperature);

    servoManager.SetNumber((int)(mqttManager.temperature * 100));
    break;
  case 2:
    lcd.setCursor(0, 0);
    lcd.print("Luftfeuchtigkeit:");
    lcd.setCursor(0, 1);
    lcd.print(mqttManager.humidity);

    servoManager.SetNumber((int)(mqttManager.humidity * 100));
    break;
  case 3:
    lcd.setCursor(0, 0);
    lcd.print("Uhrzeit MQTT");
    lcd.setCursor(0, 1);
    lcd.print(mqttManager.time);
    servoManager.SetNumber(mqttManager.time);
    break;
  case 4:
    lcd.setCursor(0, 0);
    lcd.print("Uhrzeit DCF77");
    lcd.setCursor(0, 1);
    lcd.print(String(hour())+":" + String(minute()));
    break;
  case 5:
    lcd.setCursor(0, 0);
    lcd.print("Sending MQTT");
    lcd.setCursor(0, 1);
    lcd.print("Only");
    break;
  default:
    break;
  }

  time_t DCFtime = DCF.getTime();
  
  Serial.print("Aktuelle Uhrzeit: " + String(hour())+":" + String(minute()));

  delay(300);
}
