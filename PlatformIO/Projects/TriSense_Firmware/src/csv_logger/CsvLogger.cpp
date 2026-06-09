#include <SPI.h>
#include <SD.h>
#include "Wire.h"
#include "../common/IMUSensor.h"
#include "../common/SensorFusion.h"
#include "CsvLogger.h"

static const int chipSelect = 5;
static const unsigned long sampleIntervalMs = 50; // 20 Hz sample rate
static unsigned long lastSampleTime = 0;
static File dataFile;

static void writeCsvHeader() {
  dataFile.println("time_ms,ax_g,ay_g,az_g,gx_dps,gy_dps,gz_dps,roll_deg,pitch_deg,yaw_deg");
  dataFile.flush();
}

static void writeCsvRecord(unsigned long currentTime, float ax, float ay, float az,
                           float gx, float gy, float gz,
                           float roll, float pitch, float yaw) {
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
  dataFile.print(gz, 6);
  dataFile.print(",");
  dataFile.print(roll, 2);
  dataFile.print(",");
  dataFile.print(pitch, 2);
  dataFile.print(",");
  dataFile.println(yaw, 2);
  dataFile.flush();
}

static void printSerialRecord(unsigned long currentTime, float ax, float ay, float az,
                              float gx, float gy, float gz,
                              float roll, float pitch, float yaw) {
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
  Serial.print(gz, 6);
  Serial.print(",");
  Serial.print(roll, 2);
  Serial.print(",");
  Serial.print(pitch, 2);
  Serial.print(",");
  Serial.println(yaw, 2);
}

void setupLogger() {
  Serial.begin(115200);
  while (!Serial) {
    delay(10);
  }

  Serial.println("TriSense CSV Logger starting...");

  if (!imuBegin()) {
    Serial.println("Failed to initialise IMU!");
    while (1) {
      delay(1000);
    }
  }

  Serial.println("IMU initialised.");

  imuCalibrateGyro();
  fusionInit();

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

  dataFile = SD.open("TRI_001.CSV", FILE_WRITE);

  if (!dataFile) {
    Serial.println("Failed to open TRI_001.CSV");
    while (1) {
      delay(1000);
    }
  }

  writeCsvHeader();

  Serial.println("Logging to TRI_001.CSV");
  Serial.println("time_ms,ax_g,ay_g,az_g,gx_dps,gy_dps,gz_dps,roll_deg,pitch_deg,yaw_deg");
}

void loopLogger() {
  unsigned long currentTime = millis();

  if (currentTime - lastSampleTime >= sampleIntervalMs) {
    float dt = (currentTime - lastSampleTime) / 1000.0f;
    lastSampleTime = currentTime;

    float ax, ay, az;
    float gx, gy, gz;
    imuRead(ax, ay, az, gx, gy, gz);

    fusionUpdate(ax, ay, az, gx, gy, gz, dt);
    float roll, pitch, yaw;
    fusionGetEuler(roll, pitch, yaw);

    writeCsvRecord(currentTime, ax, ay, az, gx, gy, gz, roll, pitch, yaw);
    printSerialRecord(currentTime, ax, ay, az, gx, gy, gz, roll, pitch, yaw);
  }
}
