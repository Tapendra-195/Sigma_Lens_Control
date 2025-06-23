#include "include/Firmware.h"

Firmware firmware;

void setup() {
  firmware.begin();
}

void loop() {
  firmware.run();
}