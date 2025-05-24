#include "wifi_setup.h"

const char* ssid = "WiFiSSID";
const char* password = "senha";

/**
 *@brief Função de configuração para a rede do broker.
 */
void setupWiFi() {
  delay(10);
  Serial.println();
  Serial.print("Conectando-se a ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi conectado");
  Serial.println(WiFi.localIP());
}
