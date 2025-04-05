# Projeto ESP32 LoRa com MQTT para Zabbix

Este projeto tem como objetivo adquirir dados de sensores utilizando um módulo **ESP32 LoRa**, enviando essas informações via protocolo **MQTT** para o **Zabbix**, com o **Mosquitto** funcionando como broker MQTT. Estamos avaliando a possibilidade de incluir no projeto, para torná-lo mais útil, a utilização do Dragino Gateway LoRa. 

---

## 🧩 Tecnologias Utilizadas

- **ESP32 LoRa** – Microcontrolador com suporte LoRa (Long Range) para comunicação de longo alcance  
- **MQTT (Mosquitto)** – Protocolo leve de mensagens usado para envio dos dados  
- **Zabbix** – Sistema de monitoramento de redes, utilizado para visualizar os dados  

---

## ⚙️ Instalação do Ambiente (Ubuntu)

### 0. Instalar o mySql-server

```bash
sudo apt update
sudo apt install mysql-server
```

### 1. Instalação do repositório Zabbix

```bash
sudo -s
wget https://repo.zabbix.com/zabbix/7.2/release/ubuntu/pool/main/z/zabbix-release/zabbix-release_latest_7.2+ubuntu24.04_all.deb
dpkg -i zabbix-release_latest_7.2+ubuntu24.04_all.deb
apt update
```

### 2. Instalação do servidor, o frontend e o agente Zabbix

```bash
apt install zabbix-server-mysql zabbix-frontend-php zabbix-apache-conf zabbix-sql-scripts zabbix-agent
```

### 3. Configuração do Banco de Dados (MySQL)

```bash
mysql -uroot -p
password
mysql> create database zabbix character set utf8mb4 collate utf8mb4_bin;
mysql> create user zabbix@localhost identified by 'password';
mysql> grant all privileges on zabbix.* to zabbix@localhost;
mysql> set global log_bin_trust_function_creators = 1;
mysql> quit;
```

### 4. Importando os dados iniciais ~ Esta parte pode demorar de 5 até 15 minutos.

```bash
zcat /usr/share/zabbix/sql-scripts/mysql/server.sql.gz | mysql --default-character-set=utf8mb4 -uzabbix -p zabbix
```

### 5. Desabilitar a opção 'log_bin_trust_function_creators' após importar o banco de dados

```bash
mysql -uroot -p
password
mysql> set global log_bin_trust_function_creators = 0;
mysql> quit;
```

### 6. Editar o arquivo de configuração do Zabbix

```bash
sudo gedit /etc/zabbix/zabbix_server.conf
# Atualizar:
DBPassword=password
```

### 7. Iniciar os serviços

```bash
sudo systemctl restart zabbix-server zabbix-agent apache2
sudo systemctl enable zabbix-server zabbix-agent apache2
```

### 8. Possíveis erros durante a instalação

- Tenha certeza de não interromper a fase 4 do processo, é a fase mais demorada dada a necessidade de importar uma quantidade considerável de tabelas. 
- É possível que durante o processo seja necessária a instalação da biblioteca 'libldap', para isso encontre a versão disponível no seu sistema:

```bash
sudo apt update
apt search libldap
sudo apt install libldap<versão>
```

---

## ☁️ Instalação do Mosquitto (MQTT Broker)

```bash
sudo apt install mosquitto mosquitto-clients
sudo systemctl enable mosquitto
```
---

## 🔌 Hardware Utilizado

> ⚠️ Atualizar esta parte conforme o projeto avança.

### Microcontrolador:

- Modelo: `ESP32 LoRa (ex: Heltec, TTGO, LilyGO...)`
- Frequência: `___ MHz`
- Tensão de operação: `___ V`

### Sensores:

- **Sensor 1:** `DHT22`  
  - Tipo: Temperatura / Umidade  

- **Sensor 2:** `Nome do sensor`  
  - Tipo: `___`  

### Gateway LoRa:

- Modelo: `___`

---

## 🛠️ A fazer

- [X] Escolher modelo definitivo do ESP32  
- [ ] Definir sensores e conexões  
- [ ] Criar templates no Zabbix  
- [ ] Automatizar envio/recebimento MQTT no Zabbix  
- [ ] A definir[...]  

---

## 📜 Licença

Este projeto está licenciado sob a [MIT License](LICENSE).
