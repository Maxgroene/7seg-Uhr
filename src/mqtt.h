#ifndef MQTT_H
#define MQTT_H

#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

extern const char* ssid;
extern const char* password;
extern const char* mqtt_server;
extern const char* mqtt_user;
extern const char* mqtt_password;
extern const char* mqtt_temperature_topic;
extern const char* mqtt_humidity_topic;
extern const char* mqtt_time_topic;

extern WiFiClient espClient;
extern PubSubClient client;
extern DHT dht;

#endif 
