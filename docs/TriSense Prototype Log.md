# TriSense Prototype Log

## Project aim

Build a waterproof IMU-based wearable sensor pod for swimming body-roll and stroke-rhythm analysis, with future extension to cycling and running technique feedback.

## Prototype 0.1 objective

The first prototype will prove that a small wearable sensor pod can record useful body movement data and save it locally to a microSD card for post-session analysis.

## Hardware ordered

- Seeed Studio XIAO nRF52840 Sense
- microSD module
- 3.7V 1S LiPo battery
- microSD card
- Switch/wires/headers if needed

## Success criteria for Prototype 0.1

1. XIAO powers on over USB.
2. Onboard IMU outputs acceleration and gyroscope data.
3. microSD module can create and write to a file.
4. IMU data can be saved as CSV.
5. Device can run from LiPo battery.
6. A basic lower-back movement test produces a readable graph.
7. Enclosure can pass a basic waterproof test.
8. Final prototype can record data during a swim or swim-like movement test.

---

# Test Log

## Build Prep — Components Arrived

**Date:** 18 May 2026  
**Parts received:** LiPo battery and microSD modules  
**Parts pending:** XIAO nRF52840 Sense  
**Work completed:** Initial Cirkit Designer wiring diagram created. microSD and battery checked visually.  
**Notes:** First electronics tests will be USB-powered before using the LiPo battery.  
**Next action:** Complete XIAO blink test when board arrives.

## Prep Test — microSD FAT32 Format

**Date:** 18 May 2026  
**Aim:** Prepare the microSD card for Arduino/XIAO data logging.  
**Setup:** 120GB microSD card formatted using GUIFormat.  
**Result:** Card successfully formatted as FAT32 and labelled TRISENSE.  
**Notes:** A README.txt file was added to confirm laptop read/write access.  
**Next action:** Use this card for the first microSD write test once the XIAO arrives.

## Test 001 — XIAO Blink Test

**Date:** 19 May 2026  
**Prototype version:** 0.1 Bench Test  
**Aim:** Confirm the XIAO nRF52840 Sense can be programmed over USB-C.  
**Setup:** XIAO connected to laptop via USB-C. No battery, no microSD, no enclosure.  
**Result:** Passed  
**What worked:** Blink sketch uploaded successfully after selecting the correct COM/bootloader port. Onboard LED blinked as expected.  
**What failed:** Initial upload failed because the serial port changed/disappeared during DFU upload. This was fixed by resetting the board and selecting the correct port.  
**Next action:** Run IMU serial output test.



## Test 002 — IMU Serial Output

**Date:** 19 May 2026  
**Prototype version:** 0.1 Bench Test  
**Aim:** Confirm the XIAO nRF52840 Sense onboard IMU can output acceleration and gyroscope data.  
**Setup:** XIAO connected to laptop via USB-C. No battery, no microSD, no enclosure.  
**Result:** Passed  
**What worked:** IMU initialised successfully using the Seeed Arduino LSM6DS3 library. Acceleration and gyroscope values printed to Serial Monitor.  
**What failed:** The Arduino_LSM6DS3 library failed to initialise the IMU. The Seeed LSM6DS3 library initially caused a compile error due to `SPI.setBitOrder(MSBFIRST);`; this was fixed by commenting out that line in `LSM6DS3.cpp`.  
**Next action:** Wire the microSD module and run a basic file write test. 

---

## Test 003 — microSD Write Test

**Date:** 20 May 2026  
**Prototype version:** 0.1 Bench Test  
**Aim:** Confirm the XIAO nRF52840 Sense can write data to a microSD card over SPI.  
**Setup:** XIAO powered over USB, microSD module wired over SPI, 120GB card formatted as FAT32.  
**Result:** Passed using CS on D6.  
**What worked:** SD card initialised and write test passed when chip select was moved from D7 to D6.  
**What failed:** Initial SD initialisation failed using D7 as chip select. Power to the SD module also initially failed due to breadboard rail connection issues, fixed by direct 3V3/GND wiring.  
**Final wiring:** SD 3V3 → XIAO 3V3, SD GND → XIAO GND, SD CS → XIAO D6, SD MOSI → XIAO D10, SD CLK → XIAO D8, SD MISO → XIAO D9.  
**Next action:** Combine IMU readings with microSD writing to create a timestamped CSV logger.

## Test 004 — IMU CSV Logging

**Date:** 20 May 2026  
**Prototype version:** 0.1 Bench Test  
**Aim:** Combine IMU readings with microSD writing to save timestamped motion data as a CSV file.  
**Setup:** XIAO nRF52840 Sense powered over USB, microSD module wired over SPI, onboard IMU sampled at 20 Hz.  
**Result:** Passed  
**What worked:** The XIAO successfully read acceleration and gyroscope data from the onboard IMU and saved it to a CSV file on the microSD card. The CSV was opened in Excel and plotted successfully.  
**What failed:** CS pin D7 did not initialise the SD card. D6 worked initially but later became unreliable. D5 successfully initialised the SD card and saved the CSV.  
**Final working wiring:** SD 3V3 → XIAO 3V3, SD GND → XIAO GND, SD CS → XIAO D5, SD MOSI → XIAO D10, SD CLK → XIAO D8, SD MISO → XIAO D9.  
**Next action:** Run controlled movement tests to identify which IMU axis best represents torso roll.

## Test 005 — BLE Connection Test

**Date:** 20 May 2026  
**Prototype version:** 0.1 Bench Test  
**Aim:** Confirm the XIAO nRF52840 Sense can advertise over BLE and send data to a phone.  
**Setup:** XIAO powered over USB. nRF Connect app used on phone. No battery. BLE-only test sketch uploaded.  
**Result:** Passed  
**What worked:** Phone detected the XIAO as "TriSense", connected successfully, and received counter notifications over BLE.  
**What failed:** Nothing significant in this first BLE-only test.  
**Next action:** Create an IMU BLE test that sends live acceleration and gyroscope values to the phone.

## Test 006 — BLE IMU Stream

**Date:** 20 May 2026  
**Prototype version:** 0.1 Bench Test  
**Aim:** Confirm that the XIAO nRF52840 Sense can stream live IMU data to a phone over BLE.

**Setup:** XIAO powered over USB. No LiPo. BLE IMU sketch uploaded. Phone connected using nRF Connect.

**Result:** Passed

**What worked:** The phone detected TriSense over BLE, connected successfully, and received live IMU data notifications containing timestamp, acceleration and gyroscope values.

**What failed:** Nothing significant in this test.

**Next action:** Combine microSD logging and BLE preview in one sketch, with SD as the main data logger and BLE as a low-rate live diagnostic stream.

## Test 007 — Combined SD + BLE IMU Logger

**Date:** 20 May 2026  
**Prototype version:** 0.1 Bench Test  
**Aim:** Confirm that the XIAO nRF52840 Sense can record IMU data to microSD while simultaneously streaming lower-rate live data to a phone over BLE.

**Setup:** XIAO powered over USB. microSD module connected over SPI with CS on D5. Phone connected using nRF Connect. IMU sampled and saved to CSV while BLE notifications sent live to phone.

**Result:** Passed

**What worked:** The system successfully saved timestamped acceleration and gyroscope data to the microSD card while also sending live IMU values to the phone over BLE.

**What failed:** Earlier CS pins D7 and D6 were unreliable. D5 worked for combined SD and BLE logging.

**Final working wiring:**  
SD 3V3 → XIAO 3V3  
SD GND → XIAO GND  
SD CS → XIAO D5  
SD MOSI → XIAO D10  
SD CLK → XIAO D8  
SD MISO → XIAO D9  

**Next action:** Run controlled labelled movement tests to identify which IMU axes best represent roll, pitch and yaw movements.

## Test 008 — Fixed-Rate SD + BLE Logger

**Date:** 20 May 2026  
**Prototype version:** 0.1 Bench Test  
**Aim:** Improve the combined SD + BLE logger so that microSD logging is closer to the intended 20 Hz sample rate.

**Setup:** XIAO nRF52840 Sense powered over USB, microSD module connected over SPI with CS on D5, BLE connected to phone using nRF Connect. Serial sample printing disabled and SD flushing reduced to once per second.

**Result:** Passed

**Measured performance:** 1367 samples recorded over approximately 72.0 seconds. Average sample interval was approximately 52.7 ms, giving an actual sample rate of approximately 19.0 Hz. Median sample interval was 50 ms.

**What worked:** The logger successfully recorded IMU data to microSD while simultaneously streaming BLE preview data to the phone. Removing continuous Serial printing and reducing SD flush frequency significantly improved the sample rate.

**What failed/limitations:** Occasional longer write intervals occurred, with the maximum interval around 113 ms. This is acceptable for the current prototype but may be improved later using buffered writing.

**Next action:** Run the same movement test with the XIAO temporarily mounted on the lower back/waist area to identify the true body-roll axis for swimming-style motion.

## Test 009 — Lower-Back Mounted Axis Test

**Date:** 20 May 2026  
**Prototype version:** 0.1 Breadboard Body-Mount Test  
**Aim:** Identify which IMU axis best represents torso roll when the XIAO is positioned near the lower back.

**Setup:** XIAO nRF52840 Sense and microSD module remained on the breadboard. The breadboard was positioned on the lower back while standing. Data was logged to microSD using the fixed-rate SD + BLE logger.

**Movement sequence:** Roll left/right, lean forward/back, then tilt left/right twice while standing upright.

**Result:** Passed

**Measured performance:** 1654 samples recorded over approximately 88.2 seconds. Average sample interval was approximately 53.3 ms, giving an actual sample rate of approximately 18.75 Hz. Median sample interval was 50 ms.

**Key finding:** With the current lower-back mounting orientation, torso roll is most strongly represented by `gx_dps`.

**Interpretation:** `gx_dps` should be treated as the first candidate signal for swimming body-roll detection. Forward/back leaning is more visible through changes in `ax_g` and `az_g`, while side tilting is visible through `ay_g`.

**Limitation:** The breadboard may move slightly relative to the body, so this axis mapping should be repeated once the electronics are fixed inside a case.


## Test 010 — Offline Roll Signal Processing

**Date:** 20 May 2026  
**Prototype version:** 0.1 Breadboard Body-Mount Test  
**Aim:** Process the lower-back IMU data offline to identify a usable roll signal and calculate an initial roll symmetry metric.

**Setup:** Data from the lower-back mounted breadboard test was imported into MATLAB. The `gx_dps` signal was selected as the candidate body-roll axis based on Test 009. The signal was bias-corrected, smoothed using a moving average filter, and analysed using peak detection.

**Processing steps:**
1. Loaded the CSV file recorded by the TriSense SD + BLE logger.
2. Converted `time_ms` into seconds.
3. Removed the initial settling period.
4. Selected `gx_dps` as the primary roll-axis signal.
5. Estimated and removed gyro bias.
6. Smoothed the signal using a moving average filter.
7. Detected positive and negative roll peaks.
8. Calculated an initial roll symmetry score.

**Result:** Passed

**Measured performance:**
- Estimated sample rate: 20.00 Hz
- Estimated gyro bias on `gx_dps`: 0.490 deg/s
- Mean positive roll peak: 26.66 deg/s
- Mean negative roll peak: 20.36 deg/s
- Initial roll symmetry score: 0.764

**Key finding:** The MATLAB processing pipeline successfully converted raw IMU data into a cleaned roll-axis signal and produced a basic left/right roll symmetry metric.

**Interpretation:** A symmetry score of 0.764 suggests the positive and negative roll movements were reasonably similar but not perfectly balanced. However, this should not yet be interpreted as a true swimming technique score because the test was performed manually while standing with the electronics mounted on a breadboard.

**Limitations:**
- The breadboard may have shifted slightly relative to the body.
- The movement was not a controlled swimming motion.
- The analysis was applied to a manually performed motion sequence rather than repeated stroke cycles.
- The peak detection method is basic and will need refinement later.

**Next action:** Begin designing and manufacturing a simple waterproof enclosure so the electronics can be fixed rigidly in a repeatable body-mounted orientation.



## Test 002 — microSD Write Test

**Date:**  
**Prototype version:** 0.1 Bench Test  
**Aim:** Check whether the microSD module can create and write to a file.  
**Setup:** XIAO connected to laptop over USB-C and wired to microSD module using SPI.  
**Code used:**  
**What worked:**  
**What failed:**  
**Result:** Not started  
**Next action:**  

---

## Test 003 — IMU CSV Logging

**Date:**  
**Prototype version:** 0.1 Bench Test  
**Aim:** Combine IMU reading and microSD writing to save timestamped motion data as a CSV file.  
**Setup:** XIAO connected to laptop over USB-C and wired to microSD module.  
**Expected CSV columns:** time_ms, ax, ay, az, gx, gy, gz  
**Code used:**  
**What worked:**  
**What failed:**  
**Result:** Not started  
**Next action:**  