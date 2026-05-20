#include <SPI.h>
#include <SD.h>

const int chipSelect = 5;

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);

  Serial.println("TriSense SD basic init test");

  pinMode(chipSelect, OUTPUT);
  digitalWrite(chipSelect, HIGH);

  if (!SD.begin(chipSelect)) {
    Serial.println("SD.begin failed");
    return;
  }

  Serial.println("SD.begin passed");

  File testFile = SD.open("test.txt", FILE_WRITE);

  if (testFile) {
    testFile.println("TriSense SD write test passed.");
    testFile.close();
    Serial.println("test.txt written successfully.");
  } else {
    Serial.println("Could not open test.txt");
  }
}

void loop() {}