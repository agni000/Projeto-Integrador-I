## ⚙️ Instalação do Ambiente (Ubuntu-Mint)

### 0. Instalar o MySQL-server

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
