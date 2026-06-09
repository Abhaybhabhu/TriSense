#ifndef IMUSENSOR_H
#define IMUSENSOR_H

#include "LSM6DS3.h"

bool imuBegin();
void imuCalibrateGyro(unsigned long sampleCount = 200);
void imuRead(float &ax, float &ay, float &az, float &gx, float &gy, float &gz);

#endif // IMUSENSOR_H
