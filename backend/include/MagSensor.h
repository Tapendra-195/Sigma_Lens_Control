#pragma once
#include <Arduino.h>
#include <Wire.h>
#include <functional>
#include <Adafruit_LIS3MDL.h>

/// @brief MagSensor Interface for 3-DoF Magnetometer LIS3MDL on I2C.
class MagSensor
{
public:
  explicit MagSensor(uint8_t i2cAddress = 0x1C);

  void setPowerOnHook(std::function<void(void)> fn);

  bool begin();
  bool update(bool force = false);

  float getBx_uT();
  float getBy_uT();
  float getBz_uT();
  float getBnorm_uT();

  String packCSV(uint8_t decimals = 2);

  bool isOk() const { return mOk; }

private:
  std::function<void(void)> mPowerOnHook;

  uint8_t mAddr = 0x1E;
  bool mOk = false;

  Adafruit_LIS3MDL mMag;

  float mBx = NAN, mBy = NAN, mBz = NAN; // uT
  float mOffx = 0, mOffy = 0, mOffz = 0;

  uint32_t mLastReadMs = 0;
  uint32_t mMinIntervalMs = 100; // ~10 Hz default
};
