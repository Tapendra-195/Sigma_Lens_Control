#pragma once
#include <Arduino.h>
#include <Wire.h>
#include <functional>
#include <Adafruit_LSM6DS3.h>
#include <Adafruit_Sensor.h>

/// @brief  ImuSensor Interface for Internal Measurement Unit LSM6DSOX 6-DoF IMU (accel + gyro) on I2C.
class ImuSensor
{
public:
  explicit ImuSensor(uint8_t i2cAddress = 0x6A);

  void setPowerOnHook(std::function<void(void)> fn);

  bool begin();

  // Updates cached values. Returns true if new data was read.
  bool update(bool force = false);

  // Cached values (NAN if not OK)
  float getAx(); // m/s^2
  float getAy();
  float getAz();

  float getGx(); // rad/s
  float getGy();
  float getGz();

  // Optional norms for quick health display
  float getAccelNorm(); // m/s^2
  float getGyroNorm();  // rad/s

  String packCSV(uint8_t decimals = 3);

  bool isOk() const { return mOk; }

private:
  std::function<void(void)> mPowerOnHook;

  uint8_t mAddr = 0x6A;
  bool mOk = false;

  Adafruit_LSM6DS3 mImu;

  // Cached readings
  float mAx = NAN, mAy = NAN, mAz = NAN;
  float mGx = NAN, mGy = NAN, mGz = NAN;

  // Rate limiting
  uint32_t mLastReadMs = 0;
  uint32_t mMinIntervalMs = 1000; // ~1 Hz default (tune as you like)
};
