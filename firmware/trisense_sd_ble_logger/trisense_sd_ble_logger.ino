/*
  TriSense Test 008 - Fixed-Rate SD + BLE IMU Logger

  Purpose:
  - Log IMU data to microSD at approximately 20 Hz
  - Send live BLE preview to phone at approximately 5 Hz
  - Avoid Serial printing every sample to improve sample rate
  - Flush SD every 1 second instead of every sample

  Wiring:
  SD 3V3  -> XIAO 3V3
  SD GND  -> XIAO GND
  SD CS   -> XIAO D5
  SD MOSI -> XIAO D10
  SD CLK  -> XIAO D8
  SD MISO -> XIAO D9
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
const char fileName[] = "TRI_003.CSV";

// ---------------- Timing ----------------
const unsigned long sdIntervalMs = 50;      // 20 Hz
const unsigned long bleIntervalMs = 200;    // 5 Hz
const unsigned long flushIntervalMs = 1000; // flush every 1 second

unsigned long lastSdTime = 0;
unsigned long lastBleTime = 0;
unsigned long lastFlushTime = 0;

// ---------------- Latest IMU values ----------------
float ax = 0.0;
float ay = 0.0;
float az = 0.0;
float gx = 0.0;
float gy = 0.0;
float gz = 0.0;

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

  Serial.println("TriSense Test 008 starting...");
  Serial.println("Fixed-rate SD + BLE IMU logger");

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

  dataFile = SD.open(fileName, FILE_WRITE);

  if (!dataFile) {
    Serial.println("ERROR: Failed to open CSV file.");
    while (1) {
      delay(1000);
    }
  }

  dataFile.println("time_ms,ax_g,ay_g,az_g,gx_dps,gy_dps,gz_dps");
  dataFile.flush();

  Serial.print("Logging to ");
  Serial.println(fileName);

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

    imuChar.writeValue("TriSense fixed-rate logger ready");

    BLE.advertise();

    Serial.println("BLE advertising as TriSense");
  }

  Serial.println("Setup complete.");
  Serial.println("Serial sample printing disabled for better timing.");
}

void loop() {
  unsigned long now = millis();

  // Keep BLE running
  if (bleStarted) {
    BLE.poll();
  }

  // ---------- SD logging at 20 Hz ----------
  if (now - lastSdTime >= sdIntervalMs) {
    lastSdTime = now;

    // Read IMU only when logging a sample
    ax = myIMU.readFloatAccelX();
    ay = myIMU.readFloatAccelY();
    az = myIMU.readFloatAccelZ();

    gx = myIMU.readFloatGyroX();
    gy = myIMU.readFloatGyroY();
    gz = myIMU.readFloatGyroZ();

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

  // ---------- Flush SD every 1 second ----------
  if (now - lastFlushTime >= flushIntervalMs) {
    lastFlushTime = now;
    dataFile.flush();
  }
}