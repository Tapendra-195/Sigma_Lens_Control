#pragma once

// HumiditySensor
// --------------
// Minimal wrapper around a Bosch BME280 on I2C (humidity / temperature / pressure).
//
// Teensy 4.0 default I2C pins:
//   SDA = 18, SCL = 19  (Wire)
//
// Library dependency (Arduino Library Manager):
//   - Adafruit BME280 Library
//   - Adafruit Unified Sensor (pulled in by the above)

#include <Arduino.h>
#include <functional>

class HumiditySensor
{
public:
  // Common I2C addresses for BME280 are 0x76 and 0x77.
  explicit HumiditySensor(uint8_t i2cAddress = 0x76);

  // NEW: allow firmware to provide a function that turns on sensor power
  void setPowerOnHook(std::function<void(void)> fn);

  // Call once during setup (or attach). Returns true on success.
  bool begin();

  // Force a sensor read (updates cached values). Returns true if data is valid.
  bool update(bool force = false);

  // Cached values. If sensor isn't present, these return NAN.
  float getHumidity();            // %RH
  float getTemperatureC();        // deg C
  float getPressurehPa();         // hPa (millibar)

  // A compact string intended for firmware->front-end readout.
  // Example: "H:45.2,T:21.8,P:1007.4"
  String packCSV(uint8_t decimals = 1);

  bool isOk() const { return mOk; }

private:
  std::function<void(void)> mPowerOnHook;

  uint8_t mAddr = 0x76;
  bool mOk = false;

  // Cached readings
  float mHumidity = NAN;
  float mTempC = NAN;
  float mPressurehPa = NAN;

  // Simple rate limiting (BME280 doesn't need to be polled at 60 Hz)
  uint32_t mLastReadMs = 0;
  uint32_t mMinIntervalMs = 1000; // default ~1 Hz
};
