# Projeto ESP32 LoRa com MQTT para Zabbix

Este projeto tem como objetivo adquirir dados de sensores utilizando um módulo **ESP32 LoRa**, enviando essas informações via protocolo **MQTT** para o **Zabbix**, com o **Mosquitto** funcionando como broker MQTT. A comunicação é feita localmente e permite o monitoramento em tempo real através do Zabbix, além da possibilidade de enviar comandos de controle ao dispositivo.

---

## 🧩 Tecnologias Utilizadas

- **ESP32 LoRa** – Microcontrolador com suporte LoRa (Long Range) para comunicação de longo alcance  
- **MQTT (Mosquitto)** – Protocolo leve de mensagens usado para envio dos dados  
- **Zabbix** – Sistema de monitoramento de redes, utilizado para visualizar os dados  

---

## 📦 Estrutura do Projeto

```
esp32-lora-mqtt-zabbix/
├── firmware/                # Código do ESP32
├── scripts/                 # Scripts para integração com Zabbix
├── docs/                    # Documentações, esquemas, imagens
├── README.md                # Este arquivo
```

---

## ⚙️ Instalação do Ambiente

### 1. Zabbix Server no Linux Mint

```bash
sudo apt update
sudo apt install mysql-server
sudo apt install zabbix-server-mysql zabbix-frontend-php zabbix-apache-conf zabbix-sql-scripts zabbix-agent
```

### 2. Configuração do Banco de Dados (MySQL)

```bash
sudo mysql -uroot -p

# Dentro do mysql:
CREATE DATABASE zabbix character set utf8mb4 collate utf8mb4_bin;
CREATE USER 'zabbix'@'localhost' IDENTIFIED BY 'senha_segura';
GRANT ALL PRIVILEGES ON zabbix.* TO 'zabbix'@'localhost';
FLUSH PRIVILEGES;
EXIT;
```

### 3. Importando os dados iniciais

```bash
zcat /usr/share/zabbix-sql-scripts/mysql/server.sql.gz | mysql -uzabbix -p zabbix
```

### 4. Editar o arquivo de configuração do Zabbix

```bash
sudo nano /etc/zabbix/zabbix_server.conf
# Atualizar:
DBPassword=senha_segura
```

### 5. Iniciar os serviços

```bash
sudo systemctl restart zabbix-server zabbix-agent apache2
sudo systemctl enable zabbix-server zabbix-agent apache2
```

---

## ☁️ Instalação do Mosquitto (MQTT Broker)

```bash
sudo apt install mosquitto mosquitto-clients
sudo systemctl enable mosquitto
```

### Testar a comunicação:

```bash
mosquitto_pub -t test/topic -m "Mensagem de teste"
mosquitto_sub -t test/topic
```

---

## 🔌 Hardware Utilizado

> ⚠️ Preencher esta seção posteriormente com os detalhes reais do projeto.

### Microcontrolador:

- Modelo: `ESP32 LoRa (ex: Heltec, TTGO, LilyGO...)`
- Frequência: `___ MHz`
- Tensão de operação: `___ V`

### Sensores:

- **Sensor 1:** `Nome do sensor (ex: DHT11, DHT22)`  
  - Tipo: Temperatura / Umidade  
  - Pinos utilizados: `GPIO__`

- **Sensor 2:** `Nome do sensor`  
  - Tipo: `___`  
  - Pinos utilizados: `GPIO__`

### Gateway LoRa:

- Modelo: `___`

---

## 🔄 Integração com o Zabbix

- O ESP32 publica os dados no tópico MQTT: `esp32/sensores`
- Um script ou template Zabbix com suporte a MQTT (usando por exemplo `zabbix_sender` ou `MQTT Gateway`) coleta os dados e insere no Zabbix
- Comandos de controle também podem ser enviados ao ESP32 via MQTT

---

## 📈 Exemplo de Payload (JSON)

```json
{
  "temperatura": 24.5,
  "umidade": 60.2,
  "oxigenio": 19.5
}
```

---

## 🛠️ A Fazer

- [ ] Escolher modelo definitivo do ESP32  
- [ ] Definir sensores e conexões  
- [ ] Criar templates no Zabbix  
- [ ] Automatizar envio/recebimento MQTT no Zabbix  

---

## 📜 Licença

Este projeto está licenciado sob a [MIT License](LICENSE).
