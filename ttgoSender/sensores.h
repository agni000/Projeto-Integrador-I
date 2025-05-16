#ifndef SENSORES_H
#define SENSORES_H

#include <Adafruit_MPU6050.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

struct LeituraSensores {
  float tempDHT;
  float umidDHT;
  float tempBMP;
  float pressaoBMP;
  float altitudeBMP;
  float accX, accY, accZ;
  float gyroX, gyroY, gyroZ;
};

void initSensores();
LeituraSensores lerSensores();
void publicarDadosMQTT(const LeituraSensores& leitura);

#endif