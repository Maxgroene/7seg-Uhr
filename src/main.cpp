#include <WiFi.h>
#include "Arduino.h"
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <ESP32Servo.h>
#include <PubSubClient.h>
#include "mqtt.cpp"
#include "servo.cpp"

#define DHTPIN 14
#define DHTTYPE DHT22 

const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* mqtt_server = "mqtt.mgroe.de";
const char* mqtt_user = "max";
const char* mqtt_password = "Hallo.123";
const char* mqtt_temperature_topic = "esp01/temperature";
const char* mqtt_humidity_topic = "esp01/humidity";

MQTTManager mqttManager;
WiFiClient espClient;
PubSubClient client(espClient);
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  mqttManager.setup();
}

void loop() {
  mqttManager.mqttLoop();
}
