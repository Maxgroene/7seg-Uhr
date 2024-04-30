#include "Arduino.h"
#include "mqtt.h"
#include <PubSubClient.h>
#include "display.h"
#include "led.h"

class MQTTManager {
private:
  unsigned long lastConnectionAttempt = 0;
  const unsigned long connectionInterval = 5000; 
  unsigned long lastMeasurementTime = 0;
  const unsigned long measurementInterval = 5000; 

public:
  MQTTManager() {}

  void setup() {
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);
    dht.begin();
    }

  static void callback(char* topic, byte* payload, unsigned int length) {
    
    Serial.print("Nachricht auf Topic [");
    Serial.print(topic);
    Serial.print("] Daten: ");
    for (int i = 0; i < length; i++) {
      Serial.print((char)payload[i]);
    }
    Serial.println();
  }

  void reconnect() {
    unsigned long currentMillis = millis();
    if (currentMillis - lastConnectionAttempt >= connectionInterval) {
      lastConnectionAttempt = currentMillis;

      Serial.print("Verbindung zum MQTT-Broker ...");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Verbindung zum MQTT-Broker ...");

      if (client.connect("ESP32Client", mqtt_user, mqtt_password)) {
        Serial.println(" verbunden");

        lcd.setCursor(0, 1);
        lcd.print("verbunden");

        digitalWrite(ledGreen, HIGH);
        digitalWrite(ledRed, LOW);

        client.subscribe(mqtt_temperature_topic);
        client.subscribe(mqtt_humidity_topic);
      } else {
        Serial.print(" fehlgeschlagen, rc=");
        Serial.print(client.state());
        Serial.println(" erneuter Versuch in 5 Sekunden");

        lcd.setCursor(0, 1);
        lcd.print("fehlgeschlagen");

        digitalWrite(ledRed, HIGH);
        digitalWrite(ledGreen, LOW);
      }
    }
  }

  void mqttLoop() {
    if (!client.connected()) {
      reconnect();
    }
    client.loop();

    unsigned long currentMillis = millis();
    if (currentMillis - lastMeasurementTime >= measurementInterval) {
      lastMeasurementTime = currentMillis;

      
      float humidity = dht.readHumidity();
      float temperature = dht.readTemperature();

      if (isnan(humidity) || isnan(temperature)) {
        Serial.println("Fehler beim Lesen des DHT22");
        return;
      }

      Serial.print("Luftfeuchtigkeit: ");
      Serial.print(humidity);
      Serial.print(" %\t");
      Serial.print("Temperatur: ");
      Serial.print(temperature);
      Serial.println(" *C");

    
      char tempString[8];
      dtostrf(temperature, 6, 2, tempString);
      client.publish(mqtt_temperature_topic, tempString);

      
      char humidityString[8];
      dtostrf(humidity, 6, 2, humidityString);
      client.publish(mqtt_humidity_topic, humidityString);
    }
  }
};
