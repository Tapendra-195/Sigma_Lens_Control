#include "include/CameraFirmware.h"
#include "include/Config.h"

CameraFirmware firmware;
HumiditySensor h;
ImuSensor imu;
MagSensor mag(0x1C);
DebugLevel debugLevel = DBG_INFO;

void setup() {

  Serial.begin(115200);
  // Optional but very helpful: wait up to 2 seconds for the monitor to open
  unsigned long t0 = millis();
  while (!Serial && (millis() - t0 < 2000)) {}

  Serial.println("=== SETUP START ===");
  
  firmware.attachHumiditySensor(&h);//Comment this if you are not using humidity sensor.
  firmware.attachImuSensor(&imu);
  firmware.attachMagSensor(&mag);
 
  i2cScan();
}

void loop() {
  firmware.run();
}

void i2cScan() {
  Serial.println("I2C scan start");
  for (uint8_t addr = 1; addr < 127; addr++) {
    Wire.beginTransmission(addr);
    uint8_t err = Wire.endTransmission();
    if (err == 0) {
      Serial.printf("  found 0x%02X\n", addr);
    }
  }
  Serial.println("I2C scan done");
}
