// TriSense Test 002 - IMU Serial Output
// Purpose: read acceleration and gyroscope values from the XIAO nRF52840 Sense onboard IMU.

#include <Arduino_LSM6DS3.h>

void setup() {
  Serial.begin(115200);

  while (!Serial) {
    delay(10);
  }

  if (!IMU.begin()) {
    Serial.println("Failed to initialise IMU!");
    while (1) {
      delay(1000);
    }
  }

  Serial.println("IMU initialised.");
  Serial.println("ax,ay,az,gx,gy,gz");
}

void loop() {
  float ax, ay, az;
  float gx, gy, gz;

  if (IMU.accelerationAvailable() && IMU.gyroscopeAvailable()) {
    IMU.readAcceleration(ax, ay, az);
    IMU.readGyroscope(gx, gy, gz);

    Serial.print(ax, 6);
    Serial.print(",");
    Serial.print(ay, 6);
    Serial.print(",");
    Serial.print(az, 6);
    Serial.print(",");
    Serial.print(gx, 6);
    Serial.print(",");
    Serial.print(gy, 6);
    Serial.print(",");
    Serial.println(gz, 6);
  }

  delay(50); // about 20 Hz for first test
}