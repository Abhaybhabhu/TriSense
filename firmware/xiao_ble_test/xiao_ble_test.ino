/*
  TriSense BLE Test 001
  Purpose: advertise as "TriSense" and send a counter to phone using BLE notifications.

  Phone app:
  - nRF Connect
  - Scan for "TriSense"
  - Connect
  - Enable notifications on the data characteristic
*/

#include <ArduinoBLE.h>

BLEService triSenseService("19B10000-E8F2-537E-4F6C-D104768A1214");

BLEStringCharacteristic dataChar(
  "19B10001-E8F2-537E-4F6C-D104768A1214",
  BLERead | BLENotify,
  80
);

unsigned long lastSend = 0;
int counter = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    delay(10);
  }

  Serial.println("TriSense BLE test starting...");

  if (!BLE.begin()) {
    Serial.println("BLE failed to start!");
    while (1) {
      delay(1000);
    }
  }

  BLE.setLocalName("TriSense");
  BLE.setDeviceName("TriSense");
  BLE.setAdvertisedService(triSenseService);

  triSenseService.addCharacteristic(dataChar);
  BLE.addService(triSenseService);

  dataChar.writeValue("TriSense BLE ready");

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

      if (now - lastSend >= 1000) {
        lastSend = now;

        String msg = "count," + String(counter++);
        dataChar.writeValue(msg);

        Serial.println(msg);
      }
    }

    Serial.println("Disconnected");
    BLE.advertise();
  }
}