#include <Wire.h>
#include <Adafruit_GFX.h>
#include "HT_SSD1306Wire.h"
#include <ArduinoJson.h>
#include <RadioLib.h>
#include "wifi_setup.h"
#include "mqtt_setup.h"

SSD1306Wire  factory_display(0x3c, 500000, SDA_OLED, SCL_OLED, GEOMETRY_128_64, RST_OLED);

// Pinos LoRa SX1262 para Heltec V3
#define LORA_CS   8   // NSS
#define LORA_DIO1 14
#define LORA_RST  12
#define LORA_BUSY 13

SX1262 lora = new Module(LORA_CS, LORA_DIO1, LORA_RST, LORA_BUSY);

// Variáveis globais para sensores
float temperatura = 0.0;
float umidade = 0.0;
float pressao = 0.0;

void VextON(void)
{
  pinMode(Vext,OUTPUT);
  digitalWrite(Vext, LOW);
}

void VextOFF(void) //Vext default OFF
{
  pinMode(Vext,OUTPUT);
  digitalWrite(Vext, HIGH);
}

void atualizaDisplay(float temp, float umid, float pres) {
  factory_display.clear();
  factory_display.display();  
  factory_display.setFont(ArialMT_Plain_10);
  factory_display.drawString(0, 0, "Temp: " + String(temperatura) + " C");
  factory_display.drawString(0, 12, "Umid: " + String(umidade) + " %");
  factory_display.drawString(0, 24, "Press: " + String(pressao) + " hPa");
  factory_display.display();  
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  VextON();
  delay(1000);

  factory_display.init();
  factory_display.clear();
  factory_display.display();
  pinMode(LED ,OUTPUT);
  digitalWrite(LED, LOW); 

  // Inicializa LoRa
  if (lora.begin() != RADIOLIB_ERR_NONE) {
    factory_display.clear();
    factory_display.display();  
    factory_display.setFont(ArialMT_Plain_10);
    factory_display.drawString(0, 0, "Falha de conexao LoRa!");
    factory_display.display();  
    Serial.println("Erro ao inicializar LoRa.");
    while (true);
  }

  // Configura LoRa com frequência 915 MHz
  lora.setFrequency(915.0);
  lora.setSpreadingFactor(7); // opcional: SF7 a SF12
  lora.setOutputPower(10);    // opcional

  factory_display.clear();
  factory_display.display();  
  factory_display.setFont(ArialMT_Plain_10);
  factory_display.drawString(0, 0, "Sucesso na conexao LoRa!");
  factory_display.display();  
  delay(2000);
  Serial.println("Receptor LoRa V3 pronto!");

  setupWiFi();
  setupMQTT();
}

void loop() {
  if (!mqttClient.connected()) {
    reconnectMQTT();
  }
  mqttClient.loop();
  
  String received;
  int state = lora.receive(received);

  if (state == RADIOLIB_ERR_NONE) {
    Serial.print("Recebido: ");
    Serial.println(received);

    // Decodifica JSON
    StaticJsonDocument<256> doc;
    DeserializationError error = deserializeJson(doc, received);

    if (error) {
      Serial.print("Erro JSON: ");
      Serial.println(error.c_str());
      return;
    }

    if (doc.containsKey("temp") && doc.containsKey("umid") && doc.containsKey("pres")) {
      temperatura = doc["temp"];
      umidade = doc["umid"];
      pressao = doc["pres"];

      atualizaDisplay(temperatura, umidade, pressao);

      mqttClient.publish("zabbix/temperatura", String(temperatura, 2).c_str());
      mqttClient.publish("zabbix/umidade", String(umidade, 2).c_str());
      mqttClient.publish("zabbix/pressao", String(pressao, 2).c_str());
      
    } else {
      Serial.println("Campos ausentes no JSON.");
    }
  } else if (state != RADIOLIB_ERR_RX_TIMEOUT) {
    Serial.print("Erro na recepção LoRa: ");
    Serial.println(state);
  }

  delay(10);
}
