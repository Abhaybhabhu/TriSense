/*
  TriSense Test 003 - microSD Write Test

  Purpose:
  Check whether the XIAO nRF52840 Sense can write a basic text file
  to the microSD card over SPI.

  Wiring:
  microSD GND  -> XIAO GND
  microSD VCC  -> XIAO 3V3
  microSD MISO -> XIAO D9
  microSD MOSI -> XIAO D10
  microSD SCK  -> XIAO D8
  microSD CS   -> XIAO D7
*/

#include <SPI.h>
#include <SD.h>

const int chipSelect = D7;

void setup() {
  Serial.begin(115200);

  while (!Serial) {
    delay(10);
  }

  Serial.println("TriSense microSD write test starting...");

  if (!SD.begin(chipSelect)) {
    Serial.println("SD card initialisation failed.");
    Serial.println("Check wiring, card format, and CS pin.");
    while (1) {
      delay(1000);
    }
  }

  Serial.println("SD card initialised successfully.");

  File testFile = SD.open("test.txt", FILE_WRITE);

  if (testFile) {
    testFile.println("TriSense Prototype 0.1 microSD write test");
    testFile.println("If you can read this, the XIAO wrote to the SD card successfully.");
    testFile.close();

    Serial.println("test.txt written successfully.");
  } else {
    Serial.println("Failed to open test.txt for writing.");
  }
}

void loop() {
  // Nothing needed for this test.
}