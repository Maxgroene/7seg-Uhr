#include "Arduino.h"
#include "mqtt.h"
#include <PubSubClient.h>

class MQTTManager {
private:
  unsigned long lastConnectionAttempt = 0;
  const unsigned long connectionInterval = 5000; // Zeitintervall zwischen Verbindungsversuchen in Millisekunden
  unsigned long lastMeasurementTime = 0;
  const unsigned long measurementInterval = 5000; // Zeitintervall zwischen Messungen in Millisekunden

public:
  MQTTManager() {}

  void setup() {
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);
    dht.begin();

    Serial.println();
    Serial.print("Verbindung mit ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);
  }

  static void callback(char* topic, byte* payload, unsigned int length) {
    // Funktion wird aufgerufen, wenn eine Nachricht empfangen wird
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
      if (client.connect("ESP32Client", mqtt_user, mqtt_password)) {
        Serial.println(" verbunden");
        client.subscribe(mqtt_temperature_topic);
        client.subscribe(mqtt_humidity_topic);
      } else {
        Serial.print(" fehlgeschlagen, rc=");
        Serial.print(client.state());
        Serial.println(" erneuter Versuch in 5 Sekunden");
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

      // Messung der Temperatur und Luftfeuchtigkeit
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

      // Senden der Temperaturdaten an den MQTT-Broker
      char tempString[8];
      dtostrf(temperature, 6, 2, tempString);
      client.publish(mqtt_temperature_topic, tempString);

      // Senden der Luftfeuchtigkeitsdaten an den MQTT-Broker
      char humidityString[8];
      dtostrf(humidity, 6, 2, humidityString);
      client.publish(mqtt_humidity_topic, humidityString);
    }
  }
};
