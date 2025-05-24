#include "mqtt_setup.h"

const char* mqtt_server = "MQTTIP";
WiFiClient espClient;
PubSubClient mqttClient(espClient);

/**
 *@brief  Função que configura o endereço correto do servidor MQTT.
 */
void setupMQTT() {
  mqttClient.setServer(mqtt_server, 1883);
}

/**
 *@brief Função que realiza conexão com o cliente MQTT.
 */
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
