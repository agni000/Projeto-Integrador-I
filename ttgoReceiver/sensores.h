#ifndef SENSORES_H
#define SENSORES_H

#include <Adafruit_MPU6050.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

/// Struct que contém todos os dados coletados nos sensores.
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

#endif
