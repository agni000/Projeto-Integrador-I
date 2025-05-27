# 📡 Integração MQTT → Zabbix

Esta seção descreve como os dados dos sensores recebidos via MQTT (Mosquitto) são encaminhados para o servidor Zabbix utilizando um script Python como ponte (bridge).

---

## 📜 Visão Geral

- O ESP32 publica dados nos tópicos MQTT (ex: temperatura, umidade, pressão).
- Um script Python se conecta ao broker Mosquitto, escuta esses tópicos e envia os valores para o Zabbix.
- A integração utiliza as bibliotecas `paho-mqtt` e `pyzabbix`.

---

## 🛠️ Dependências

Certifique-se de instalar as bibliotecas necessárias com:

```bash
pip install paho-mqtt pyzabbix
```

---

## ⚙️ Configuração do Zabbix

1. No frontend do Zabbix, crie um **Host** com o nome exato usado no script.  
   Exemplo: `ESP32_SENSOR`

2. Crie **Itens** do tipo **Zabbix trapper** com as seguintes chaves:
   - `temperatura`
   - `umidade`
   - `pressao`

---

## 🧠 Script Python

```python
import paho.mqtt.client as mqtt
from pyzabbix import ZabbixSender, ZabbixMetric

ZABBIX_SERVER = '127.0.0.1'
ZABBIX_HOST = 'ESP32_SENSOR'
MQTT_TOPICS = [("zabbix/temperatura", 0), ("zabbix/umidade", 0), ("zabbix/pressao", 0), ("zabbix/accX", 0), ("zabbix/accY", 0), ("zabbix/accZ", 0)]

def on_message(client, userdata, msg):
    try:
        payload = msg.payload.decode()
        key = msg.topic.replace("zabbix/", "")
        print(f"[MQTT] Recebido: {key} = {payload}")

        metric = ZabbixMetric(ZABBIX_HOST, key, payload)
        result = ZabbixSender(zabbix_server=ZABBIX_SERVER).send([metric])

        if result.failed:
            print("❌ Falha ao enviar para o Zabbix")
        else:
            print("✅ Valor enviado com sucesso")

    except Exception as e:
        print("❗ Erro ao processar a mensagem:", e)

client = mqtt.Client()
client.on_message = on_message
client.connect("localhost", 1883, 60)
client.subscribe(MQTT_TOPICS)

print("🚀 Escutando MQTT e enviando dados para o Zabbix...")
client.loop_forever()
```

---

## 🏃‍♂️ Execução

Execute o script com:

```bash
python3 nome_do_arquivo.py
```
