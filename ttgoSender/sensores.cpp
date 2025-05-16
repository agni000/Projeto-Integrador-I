#include "sensores.h"
#include "mqtt_setup.h"

Adafruit_MPU6050 mpu;
Adafruit_BMP280 bmp;
#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

void initSensores() {
  if (!mpu.begin()) {
    Serial.println("Erro ao iniciar o MPU6050!");
    while (1);
  }

  if (!bmp.begin(0x76)) {
    Serial.println("Erro ao iniciar o BMP280!");
    while (1);
  }

  bmp.setSampling(
    Adafruit_BMP280::MODE_NORMAL,
    Adafruit_BMP280::SAMPLING_X2,
    Adafruit_BMP280::SAMPLING_X16,
    Adafruit_BMP280::FILTER_X16,
    Adafruit_BMP280::STANDBY_MS_500
  );

  dht.begin();

  Serial.println("Sensores iniciados com sucesso.");
}

LeituraSensores lerSensores() {
  LeituraSensores leitura;

  sensors_event_t accel, gyro, temp;
  mpu.getEvent(&accel, &gyro, &temp);

  leitura.accX = accel.acceleration.x;
  leitura.accY = accel.acceleration.y;
  leitura.accZ = accel.acceleration.z;
  leitura.gyroX = gyro.gyro.x;
  leitura.gyroY = gyro.gyro.y;
  leitura.gyroZ = gyro.gyro.z;

  leitura.tempBMP = bmp.readTemperature();
  leitura.pressaoBMP = bmp.readPressure() / 100.0;
  leitura.altitudeBMP = bmp.readAltitude(1013.25);

  leitura.tempDHT = dht.readTemperature();
  leitura.umidDHT = dht.readHumidity();

  if (isnan(leitura.tempDHT) || isnan(leitura.umidDHT)) {
    Serial.println("Erro ao ler o DHT22.");
  }

  return leitura;
}

void publicarDadosMQTT(const LeituraSensores& l) {
  mqttClient.publish("zabbix/temperatura", String(l.tempDHT, 2).c_str());
  mqttClient.publish("zabbix/umidade", String(l.umidDHT, 2).c_str());
  mqttClient.publish("zabbix/pressao", String(l.pressaoBMP, 2).c_str());
}