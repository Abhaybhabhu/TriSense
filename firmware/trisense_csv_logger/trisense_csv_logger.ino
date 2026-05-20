/*
  TriSense Test 004 - IMU CSV Logger

  Purpose:
  - Read acceleration and gyroscope data from XIAO nRF52840 Sense onboard IMU
  - Save timestamped data to microSD card as CSV

  Wiring:
  SD 3V3  -> XIAO 3V3
  SD GND  -> XIAO GND
  SD CS   -> XIAO D6
  SD MOSI -> XIAO D10
  SD CLK  -> XIAO D8
  SD MISO -> XIAO D9
*/

#include <SPI.h>
#include <SD.h>
#include "LSM6DS3.h"
#include "Wire.h"

LSM6DS3 myIMU(I2C_MODE, 0x6A);

const int chipSelect = 5;
const unsigned long sampleIntervalMs = 50; // 20 Hz sample rate

unsigned long lastSampleTime = 0;
File dataFile;

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    delay(10);
  }

  Serial.println("TriSense CSV Logger starting...");

  // Initialise IMU
  if (myIMU.begin() != 0) {
    Serial.println("Failed to initialise IMU!");
    while (1) {
      delay(1000);
    }
  }

  Serial.println("IMU initialised.");

  // Initialise SD card
  pinMode(chipSelect, OUTPUT);
  digitalWrite(chipSelect, HIGH);
  SPI.begin();

  if (!SD.begin(chipSelect)) {
    Serial.println("microSD initialisation failed!");
    Serial.println("Check wiring, SD card, and CS pin.");
    while (1) {
      delay(1000);
    }
  }

  Serial.println("microSD initialised.");

  // Open CSV file
  dataFile = SD.open("TRI_001.CSV", FILE_WRITE);

  if (!dataFile) {
    Serial.println("Failed to open TRI_001.CSV");
    while (1) {
      delay(1000);
    }
  }

  // Write CSV header
  dataFile.println("time_ms,ax_g,ay_g,az_g,gx_dps,gy_dps,gz_dps");
  dataFile.flush();

  Serial.println("Logging to TRI_001.CSV");
  Serial.println("time_ms,ax_g,ay_g,az_g,gx_dps,gy_dps,gz_dps");
}

void loop() {
  unsigned long currentTime = millis();

  if (currentTime - lastSampleTime >= sampleIntervalMs) {
    lastSampleTime = currentTime;

    float ax = myIMU.readFloatAccelX();
    float ay = myIMU.readFloatAccelY();
    float az = myIMU.readFloatAccelZ();

    float gx = myIMU.readFloatGyroX();
    float gy = myIMU.readFloatGyroY();
    float gz = myIMU.readFloatGyroZ();

    // Write to SD card
    dataFile.print(currentTime);
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

    // Flush regularly so data is saved even if power is removed
    dataFile.flush();

    // Also print to Serial for live checking
    Serial.print(currentTime);
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
}