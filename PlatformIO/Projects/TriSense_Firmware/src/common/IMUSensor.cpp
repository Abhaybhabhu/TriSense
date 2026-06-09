#include "IMUSensor.h"
#include <Arduino.h>

LSM6DS3 myIMU(I2C_MODE, 0x6A);

static float gyroBiasX = 0.0f;
static float gyroBiasY = 0.0f;
static float gyroBiasZ = 0.0f;

bool imuBegin() {
  return myIMU.begin() == 0;
}

void imuCalibrateGyro(unsigned long sampleCount) {
  gyroBiasX = 0.0f;
  gyroBiasY = 0.0f;
  gyroBiasZ = 0.0f;

  for (unsigned long i = 0; i < sampleCount; ++i) {
    gyroBiasX += myIMU.readFloatGyroX();
    gyroBiasY += myIMU.readFloatGyroY();
    gyroBiasZ += myIMU.readFloatGyroZ();
    delay(10);
  }

  gyroBiasX /= sampleCount;
  gyroBiasY /= sampleCount;
  gyroBiasZ /= sampleCount;
}

void imuRead(float &ax, float &ay, float &az, float &gx, float &gy, float &gz) {
  ax = myIMU.readFloatAccelX();
  ay = myIMU.readFloatAccelY();
  az = myIMU.readFloatAccelZ();

  gx = myIMU.readFloatGyroX() - gyroBiasX;
  gy = myIMU.readFloatGyroY() - gyroBiasY;
  gz = myIMU.readFloatGyroZ() - gyroBiasZ;
}
