/*
  TriSense Test 006 - BLE IMU Stream

  Purpose:
  - Read IMU data from XIAO nRF52840 Sense
  - Send acceleration and gyroscope values to phone over BLE
  - View live data using nRF Connect

  Phone:
  - Open nRF Connect
  - Scan for "TriSense"
  - Connect
  - Enable notifications on characteristic:
    19B10001-E8F2-537E-4F6C-D104768A1214
*/

#include <ArduinoBLE.h>
#include "LSM6DS3.h"
#include "Wire.h"

LSM6DS3 myIMU(I2C_MODE, 0x6A);

BLEService triSenseService("19B10000-E8F2-537E-4F6C-D104768A1214");

BLEStringCharacteristic imuChar(
  "19B10001-E8F2-537E-4F6C-D104768A1214",
  BLERead | BLENotify,
  120
);

unsigned long lastSendTime = 0;
const unsigned long bleIntervalMs = 200; // 5 Hz BLE stream

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    delay(10);
  }

  Serial.println("TriSense BLE IMU test starting...");

  if (myIMU.begin() != 0) {
    Serial.println("Failed to initialise IMU!");
    while (1) {
      delay(1000);
    }
  }

  Serial.println("IMU initialised.");

  if (!BLE.begin()) {
    Serial.println("BLE failed to start!");
    while (1) {
      delay(1000);
    }
  }

  BLE.setLocalName("TriSense");
  BLE.setDeviceName("TriSense");
  BLE.setAdvertisedService(triSenseService);

  triSenseService.addCharacteristic(imuChar);
  BLE.addService(triSenseService);

  imuChar.writeValue("TriSense IMU ready");

  BLE.advertise();

  Serial.println("BLE advertising as TriSense");
}

void loop() {
  BLEDevice central = BLE.central();

  if (central) {
    Serial.print("Connected to: ");
    Serial.println(central.address());

    while (central.connected()) {
      unsigned long now = millis();

      if (now - lastSendTime >= bleIntervalMs) {
        lastSendTime = now;

        float ax = myIMU.readFloatAccelX();
        float ay = myIMU.readFloatAccelY();
        float az = myIMU.readFloatAccelZ();

        float gx = myIMU.readFloatGyroX();
        float gy = myIMU.readFloatGyroY();
        float gz = myIMU.readFloatGyroZ();

        String msg = String(now) + "," +
                     String(ax, 3) + "," +
                     String(ay, 3) + "," +
                     String(az, 3) + "," +
                     String(gx, 2) + "," +
                     String(gy, 2) + "," +
                     String(gz, 2);

        imuChar.writeValue(msg);
        Serial.println(msg);
      }
    }

    Serial.println("Disconnected");
    BLE.advertise();
  }
}