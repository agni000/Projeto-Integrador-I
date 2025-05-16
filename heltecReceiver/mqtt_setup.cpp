#include "mqtt_setup.h"

const char* mqtt_server = "192.168.0.115";

WiFiClient espClient;
PubSubClient mqttClient(espClient);

void setupMQTT() {
  mqttClient.setServer(mqtt_server, 1883);
}

void reconnectMQTT() {
  while (!mqttClient.connected()) {
    Serial.print("Conectando no MQTT...");
    if (mqttClient.connect("ESP32ClientII")) {
      Serial.println("conectado");
    } else {
      Serial.print("falhou, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" tentando novamente em 5s");
      delay(5000);
    }
  }
}
