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

## Test 001 — IMU Serial Output

**Date:**  
**Prototype version:** 0.1 Bench Test  
**Aim:** Check whether the XIAO nRF52840 Sense can read onboard IMU data and print it to the Serial Monitor.  
**Setup:** XIAO connected to laptop over USB-C. No microSD, no battery, no enclosure.  
**Code used:**  
**What worked:**  
**What failed:**  
**Result:** Not started  
**Next action:**  

---

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