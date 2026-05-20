/*
  TriSense Test 007 - SD + BLE IMU Logger

  Purpose:
  - Read acceleration and gyroscope data from XIAO nRF52840 Sense onboard IMU
  - Save full-resolution timestamped data to microSD as CSV
  - Send lower-rate live preview data to phone over BLE

  Wiring:
  SD 3V3  -> XIAO 3V3
  SD GND  -> XIAO GND
  SD CS   -> XIAO D5
  SD MOSI -> XIAO D10
  SD CLK  -> XIAO D8
  SD MISO -> XIAO D9

  Phone:
  - Use nRF Connect
  - Scan for "TriSense"
  - Connect
  - Enable notifications on characteristic:
    19B10001-E8F2-537E-4F6C-D104768A1214
*/

#include <SPI.h>
#include <SD.h>
#include <ArduinoBLE.h>
#include "LSM6DS3.h"
#include "Wire.h"

// ---------------- IMU ----------------
LSM6DS3 myIMU(I2C_MODE, 0x6A);

// ---------------- SD ----------------
const int chipSelect = 5;
File dataFile;

// ---------------- Timing ----------------
const unsigned long sdIntervalMs = 50;    // 20 Hz SD logging
const unsigned long bleIntervalMs = 200;  // 5 Hz BLE preview

unsigned long lastSdTime = 0;
unsigned long lastBleTime = 0;

// ---------------- BLE ----------------
BLEService triSenseService("19B10000-E8F2-537E-4F6C-D104768A1214");

BLEStringCharacteristic imuChar(
  "19B10001-E8F2-537E-4F6C-D104768A1214",
  BLERead | BLENotify,
  120
);

bool bleStarted = false;

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    delay(10);
  }

  Serial.println("TriSense SD + BLE Logger starting...");

  // ---------- IMU setup ----------
  if (myIMU.begin() != 0) {
    Serial.println("ERROR: Failed to initialise IMU!");
    while (1) {
      delay(1000);
    }
  }

  Serial.println("IMU initialised.");

  // ---------- SD setup ----------
  pinMode(chipSelect, OUTPUT);
  digitalWrite(chipSelect, HIGH);
  SPI.begin();

  if (!SD.begin(chipSelect)) {
    Serial.println("ERROR: microSD initialisation failed!");
    Serial.println("Check wiring, SD card, and CS pin.");
    while (1) {
      delay(1000);
    }
  }

  Serial.println("microSD initialised.");

  dataFile = SD.open("TRI_002.CSV", FILE_WRITE);

  if (!dataFile) {
    Serial.println("ERROR: Failed to open TRI_002.CSV");
    while (1) {
      delay(1000);
    }
  }

  dataFile.println("time_ms,ax_g,ay_g,az_g,gx_dps,gy_dps,gz_dps");
  dataFile.flush();

  Serial.println("Logging to TRI_002.CSV");

  // ---------- BLE setup ----------
  if (!BLE.begin()) {
    Serial.println("WARNING: BLE failed to start. Continuing with SD logging only.");
    bleStarted = false;
  } else {
    bleStarted = true;

    BLE.setLocalName("TriSense");
    BLE.setDeviceName("TriSense");
    BLE.setAdvertisedService(triSenseService);

    triSenseService.addCharacteristic(imuChar);
    BLE.addService(triSenseService);

    imuChar.writeValue("TriSense SD+BLE ready");

    BLE.advertise();

    Serial.println("BLE advertising as TriSense");
  }

  Serial.println("time_ms,ax_g,ay_g,az_g,gx_dps,gy_dps,gz_dps");
}

void loop() {
  unsigned long now = millis();

  // Keep BLE stack alive
  if (bleStarted) {
    BLE.poll();
  }

  // Read IMU every loop when needed
  float ax = myIMU.readFloatAccelX();
  float ay = myIMU.readFloatAccelY();
  float az = myIMU.readFloatAccelZ();

  float gx = myIMU.readFloatGyroX();
  float gy = myIMU.readFloatGyroY();
  float gz = myIMU.readFloatGyroZ();

  // ---------- SD logging at 20 Hz ----------
  if (now - lastSdTime >= sdIntervalMs) {
    lastSdTime = now;

    dataFile.print(now);
    dataFile.print(",");
    dataFile.print(ax, 6);
    dataFile.print(",");
    dataFile.print(ay, 6);
    dataFile.print(",");
    dataFile.print(az, 6);
    dataFile.print(",");
    dataFile.print(gx, 6);
    dataFile.print(",");
    dataFile.print(gy, 6);
    dataFile.print(",");
    dataFile.println(gz, 6);

    // Flush every sample for safety during testing.
    // Later we can flush less often to improve performance.
    dataFile.flush();

    Serial.print(now);
    Serial.print(",");
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

  // ---------- BLE preview at 5 Hz ----------
  if (bleStarted && BLE.connected() && (now - lastBleTime >= bleIntervalMs)) {
    lastBleTime = now;

    String msg = String(now) + "," +
                 String(ax, 3) + "," +
                 String(ay, 3) + "," +
                 String(az, 3) + "," +
                 String(gx, 2) + "," +
                 String(gy, 2) + "," +
                 String(gz, 2);

    imuChar.writeValue(msg);
  }
}