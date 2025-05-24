#include <Wire.h>
#include <Adafruit_GFX.h>
#include "HT_SSD1306Wire.h"
#include <ArduinoJson.h>
#include <RadioLib.h>
#include "wifi_setup.h"
#include "mqtt_setup.h"
#include "sensores.h"

// Pinos LoRa SX1262 para Heltec V3
#define LORA_CS   8   ///< Chip select do LoRa
#define LORA_DIO1 14  ///< Pino DIO1 do LoRa
#define LORA_RST  12  ///< Pino de reset do LoRa
#define LORA_BUSY 13  ///< Pino de busy do LoRa

// Pinos OLED
#define SDA_OLED 17   ///< Pino SDA para display OLED
#define SCL_OLED 18   ///< Pino SCL para display OLED
#define RST_OLED 21   ///< Pino de reset do OLED

/// Instância do módulo LoRa SX1262
SX1262 lora = new Module(LORA_CS, LORA_DIO1, LORA_RST, LORA_BUSY);

/// Instância do display OLED usando HT_SSD1306Wire
SSD1306Wire factory_display(0x3c, 500000, SDA_OLED, SCL_OLED, GEOMETRY_128_64, RST_OLED);

/// Criação de um segundo barramento I2C para sensores
TwoWire sensoresI2C = TwoWire(1);

/// Variáveis globais para armazenar leituras dos sensores
float temperatura = 0.0;
float umidade = 0.0;
float pressao = 0.0;

/**
 * @brief Liga a alimentação dos periféricos (Vext).
 */
void VextON(void)
{
  pinMode(Vext, OUTPUT);
  digitalWrite(Vext, LOW);
}

/**
 * @brief Desliga a alimentação dos periféricos (Vext).
 */
void VextOFF(void)
{
  pinMode(Vext, OUTPUT);
  digitalWrite(Vext, HIGH);
}

/**
 * @brief Atualiza o display OLED com os dados dos sensores.
 * @param temp Temperatura em graus Celsius.
 * @param umid Umidade relativa do ar em porcentagem.
 * @param pres Pressão atmosférica em hPa.
 */
void atualizaDisplay(float temp, float umid, float pres) {
  factory_display.clear();
  factory_display.display();  
  factory_display.setFont(ArialMT_Plain_10);
  factory_display.drawString(0, 0, "Temp: " + String(temp) + " C");
  factory_display.drawString(0, 12, "Umid: " + String(umid) + " %");
  factory_display.drawString(0, 24, "Press: " + String(pres) + " hPa");
  factory_display.display();  
}

/**
 * @brief Função de configuração. Inicializa sensores, LoRa e display.
 */
void setup() {
  Serial.begin(115200);
  delay(1000);

  VextON();
  delay(1000);
  
  sensoresI2C.begin(41, 42);
  delay(100);
  initSensores();
  
  factory_display.init();
  factory_display.clear();
  factory_display.display();
  pinMode(LED , OUTPUT);
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

  // Configuração adicional do LoRa
  lora.setFrequency(915.0);
  lora.setSpreadingFactor(7);
  lora.setOutputPower(10);

  factory_display.clear();
  factory_display.setFont(ArialMT_Plain_10);
  factory_display.drawString(0, 0, "Sucesso na conexao LoRa!");
  factory_display.display();  
  delay(2000);
  Serial.println("Receptor LoRa V3 pronto!");
}

/**
 * @brief Função principal em loop. Lê sensores, envia dados via LoRa e atualiza o display.
 */
void loop() { 
  LeituraSensores leitura = lerSensores();

  temperatura = leitura.tempDHT;
  umidade = leitura.umidDHT;
  pressao = leitura.pressaoBMP;

  atualizaDisplay(temperatura, umidade, pressao);

  StaticJsonDocument<128> doc;
  
  doc["temp"] = temperatura;
  doc["umid"] = umidade;
  doc["pres"] = pressao;

  char buffer[128];
  serializeJson(doc, buffer);
  
  int state = lora.transmit((uint8_t*)buffer, strlen(buffer));

  if (state == RADIOLIB_ERR_NONE) {
    Serial.println("Mensagem enviada com sucesso via LoRa!");
  } else {
    Serial.print("Erro ao enviar via LoRa: ");
    Serial.println(state);
  }

  Serial.println(buffer); // debug  
  delay(2000);
}
