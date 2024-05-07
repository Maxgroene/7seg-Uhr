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
    {178, 150, 130, 110, 90, 70, 50, 35, 20, 0},
    {178, 160, 130, 110, 90, 70, 55, 30, 17, 0},
    {178, 150, 130, 110, 90, 70, 55, 35, 17, 0},
    {178, 150, 130, 110, 90, 70, 50, 35, 20, 0},
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

const char *ssid = "BKR-HotSpot";
const char *password = "";
const char *mqtt_server = "mqtt.mgroe.de";
const char *mqtt_user = "max";
const char *mqtt_password = "Hallo.123";
const char *mqtt_temperature_topic = "esp01/temperature";
const char *mqtt_humidity_topic = "esp01/humidity";

ServoManager servoManager;
MQTTManager mqttManager;
WiFiClient espClient;
PubSubClient client(espClient);
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(lcdAddress, lcdColumns, lcdRows);

int test = 0;

void setup()
{
  pinMode(ledGreen, OUTPUT);
  pinMode(ledRed, OUTPUT);

  lcd.init();
  lcd.backlight();

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
  int tempPot = map(potValue, 0, 4095, 0, 3);

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
    lcd.print("Sending MQTT");
    lcd.setCursor(0, 1);
    lcd.print("Only");
    break;
  default:
    break;
  }

  delay(300);
}
