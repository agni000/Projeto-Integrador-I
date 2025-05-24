#include <Wire.h>
#include <SPI.h>
#include <LoRa.h>
#include <ArduinoJson.h>
#include "wifi_setup.h"
#include "mqtt_setup.h"

// Pinos LoRa do TTGO
#define SCK     5     ///<  LoRa SCK
#define MISO    19    ///<  LoRa MISO
#define MOSI    27    ///<  LoRa MOSI
#define SS      18    ///<  LoRa CS 
#define RST     23    ///<  LoRa RESET
#define DIO0    26    ///<  IRQ do LoRa

/**
 * @brief Função de inicialização do sistema.
 *
 * Inicializa a comunicação serial, configura os pinos do LoRa,
 * inicia a comunicação LoRa e conecta ao Wi-Fi e MQTT.
 */
void setup() {
  Serial.begin(115200);
  delay(1000);
  LoRa.setPins(SS, RST, DIO0);
  
  if (!LoRa.begin(915E6)) {
    Serial.println("Falha de conexao LoRa!");
    while(1);
  }
  Serial.println("Transmissor LoRa pronto!");

  setupWiFi();
  setupMQTT();
}

/**
 * @brief Loop principal. Verifica conexão MQTT, recebe pacotes LoRa, decodifica JSON e publica via MQTT.
 *
 */
void loop() {
  if (!mqttClient.connected()) {
    reconnectMQTT();
  }
  mqttClient.loop();

  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    String received = "";
    while (LoRa.available()) {
      received += (char)LoRa.read();
    }

    Serial.print("Recebido via LoRa: ");
    Serial.println(received);

    /// Tenta decodificar o JSON
    StaticJsonDocument<256> doc;
    DeserializationError error = deserializeJson(doc, received);
    
    if (error) {
      Serial.print("Erro ao decodificar JSON: ");
      Serial.println(error.c_str());
      return;
    }

    /// Se dados válidos forem recebidos
    if (doc.containsKey("temp") && doc.containsKey("umid") && doc.containsKey("pres")) {
      float temperatura = doc["temp"];
      float umidade = doc["umid"];
      float pressao = doc["pres"];

      Serial.println("Dados decodificados:");
      Serial.print("Temp: "); Serial.println(temperatura);
      Serial.print("Umid: "); Serial.println(umidade);
      Serial.print("Press: "); Serial.println(pressao);

      /// Envia via MQTT para tópicos que o Zabbix espera
      mqttClient.publish("zabbix/temperatura", String(temperatura, 2).c_str());
      mqttClient.publish("zabbix/umidade", String(umidade, 2).c_str());
      mqttClient.publish("zabbix/pressao", String(pressao, 2).c_str());
    } else {
      Serial.println("JSON recebido sem todas as chaves esperadas.");
    }
  }
  
  /// Delay que evita sobrecarga do microcontrolador	
  delay(10);
}
