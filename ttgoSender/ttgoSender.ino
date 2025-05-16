#include <Wire.h>
#include <SPI.h>
#include <LoRa.h>
#include <ArduinoJson.h>
#include "sensores.h"

#define SCK     5     // LoRa SCK
#define MISO    19    // LoRa MISO
#define MOSI    27    // LoRa MOSI
#define SS      18    // LoRa CS (às vezes chamado de NSS)
#define RST     23    // LoRa RESET (diferente de alguns tutoriais)
#define DIO0    26    // IRQ do LoRa

void setup() {
  Serial.begin(115200);
  delay(1000);
  LoRa.setPins(SS, RST, DIO0);
  
  Wire.begin(21, 22); // SDA, SCL
  initSensores();

  if (!LoRa.begin(915E6)) {
    Serial.println("Falha de conexao LoRa!");
    while(1);
  }
  Serial.println("Transmissor LoRa pronto!");
}

void loop() {

  LeituraSensores leitura = lerSensores();

  StaticJsonDocument<128> doc;
  
  doc["temp"] = leitura.tempDHT;
  doc["umid"] = leitura.umidDHT;
  doc["pres"] = leitura.pressaoBMP;

  char buffer[128];
  serializeJson(doc, buffer);

  LoRa.beginPacket();
  LoRa.print(buffer);  // envia o JSON como texto
  LoRa.endPacket();

  Serial.println(buffer); // debug
  
  delay(2000);
}
