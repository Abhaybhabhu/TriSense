#include "SensorFusion.h"
#include <math.h>

static float rollAngle = 0.0f;
static float pitchAngle = 0.0f;
static float yawAngle = 0.0f;
static float fusionAlpha = 0.98f;
static bool fusionInitialized = false;

static const float RAD_TO_DEG = 57.29577951308232f;

void fusionInit(float sampleFreq) {
  (void)sampleFreq;
  rollAngle = 0.0f;
  pitchAngle = 0.0f;
  yawAngle = 0.0f;
  fusionInitialized = false;
}

void fusionUpdate(float ax, float ay, float az, float gx, float gy, float gz, float dt) {
  if (dt <= 0.0f) {
    return;
  }

  float accelRoll = atan2(ay, az) * RAD_TO_DEG;
  float accelPitch = atan2(-ax, sqrt(ay * ay + az * az)) * RAD_TO_DEG;

  if (!fusionInitialized) {
    rollAngle = accelRoll;
    pitchAngle = accelPitch;
    yawAngle = 0.0f;
    fusionInitialized = true;
  } else {
    rollAngle = fusionAlpha * (rollAngle + gx * dt) + (1.0f - fusionAlpha) * accelRoll;
    pitchAngle = fusionAlpha * (pitchAngle + gy * dt) + (1.0f - fusionAlpha) * accelPitch;
    yawAngle += gz * dt;

    if (yawAngle > 180.0f) {
      yawAngle -= 360.0f;
    } else if (yawAngle < -180.0f) {
      yawAngle += 360.0f;
    }
  }
}

void fusionGetEuler(float &roll, float &pitch, float &yaw) {
  roll = rollAngle;
  pitch = pitchAngle;
  yaw = yawAngle;
}
