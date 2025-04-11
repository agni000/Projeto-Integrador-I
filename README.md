# Projeto ESP32 LoRa com MQTT para Zabbix

Este projeto tem como objetivo adquirir dados de sensores utilizando um módulo **ESP32 LoRa**, enviando essas informações via protocolo **MQTT** para o **Zabbix**, com o **Mosquitto** funcionando como broker MQTT. Estamos avaliando a possibilidade de incluir no projeto, para torná-lo mais útil, a utilização do Dragino Gateway LoRa. 

---

## 🧩 Tecnologias Utilizadas

- **ESP32 LoRa** – Microcontrolador com suporte LoRa (Long Range) para comunicação de longo alcance  
- **MQTT (Mosquitto)** – Protocolo leve de mensagens usado para envio dos dados  
- **Zabbix** – Sistema de monitoramento de redes, utilizado para visualizar os dados  

---

## 🔌 Hardware Utilizado

> ⚠️ Atualizar esta parte conforme o projeto avança.

### Microcontrolador:

- Modelo: `ESP32 LoRa TTGO-LoRa T-Beam`
- Referência: https://i.imgur.com/dnENtkw.jpeg

### Sensores:

- **Sensor 1:** `DHT22`  
  - Tipo: Temperatura / Umidade  
  - Referência: https://www.alldatasheet.com/datasheet-pdf/view/1132459/ETC2/DHT22.html

- **Sensor 2:** `Nome do sensor`  
  - Tipo: `___`  
  - Referência: `___`  
  
  
### Gateway LoRa:

- Modelo: `___`

---

## Documentação

- 📘 [Configuração do Zabbix](ZABBIX.md)

---

## 📜 Licença

Este projeto está licenciado sob a [MIT License](LICENSE).
