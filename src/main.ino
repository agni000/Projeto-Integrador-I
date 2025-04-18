#include <Wire.h>
#include "wifi_setup.h"
#include "mqtt_setup.h"
#include "sensores.h"

void setup() {
  Serial.begin(115200);
  delay(1000);

  setupWiFi();
  setupMQTT();

  Wire.begin(21, 22); // SDA, SCL
  initSensores();
}

void loop() {
  if (!mqttClient.connected()) {
    reconnectMQTT();
  }
  mqttClient.loop();

  LeituraSensores leitura = lerSensores();
  publicarDadosMQTT(leitura);

  delay(2000);
}