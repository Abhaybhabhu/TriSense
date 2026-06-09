#ifndef SENSORFUSION_H
#define SENSORFUSION_H

void fusionInit(float sampleFreq = 50.0f);
void fusionUpdate(float ax, float ay, float az, float gx, float gy, float gz, float dt);
void fusionGetEuler(float &roll, float &pitch, float &yaw);

#endif // SENSORFUSION_H
