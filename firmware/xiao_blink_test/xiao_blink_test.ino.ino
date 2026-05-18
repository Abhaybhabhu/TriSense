// TriSense Test 001 - XIAO Blink Test
// Purpose: confirm the XIAO nRF52840 Sense can be programmed over USB-C.

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_BUILTIN, LOW);   // Many XIAO onboard LEDs are active-low
  delay(500);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
}