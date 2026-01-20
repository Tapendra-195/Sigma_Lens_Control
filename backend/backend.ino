#include "include/CameraFirmware.h"

CameraFirmware firmware;
HumiditySensor h;

void setup() {

  Serial.begin(115200);
  // Optional but very helpful: wait up to 2 seconds for the monitor to open
  unsigned long t0 = millis();
  while (!Serial && (millis() - t0 < 2000)) {}

  Serial.println("=== SETUP START ===");
  
  firmware.attachHumiditySensor(&h);//Comment this if you are not using humidity sensor.
}

void loop() {
  firmware.run();
}