#ifndef MQTT_SETUP_H
#define MQTT_SETUP_H

#include <PubSubClient.h>
#include <WiFi.h>

extern WiFiClient espClient;
extern PubSubClient mqttClient;

void setupMQTT();
void reconnectMQTT();

#endif