#include "include/CameraFirmware.h"

CameraFirmware firmware;
HumiditySensor h;

void setup() {
  
  firmware.attachHumiditySensor(&h);//Comment this if you are not using humidity sensor.
}

void loop() {
  firmware.run();
}