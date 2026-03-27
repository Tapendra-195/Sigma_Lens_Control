#include "../include/ImuSensor.h"
#include <math.h>

ImuSensor::ImuSensor(uint8_t i2cAddress) : mAddr(i2cAddress) {}

void ImuSensor::setPowerOnHook(std::function<void(void)> fn) { mPowerOnHook = fn; }

bool ImuSensor::begin()
{
  if (mPowerOnHook) {
    mPowerOnHook();          // ensure TPS22918 EN is high
    delay(20);               // allow rail + BME280 to come up
  } else {
    Serial.println("IMU: no power hook");
  }
  Wire.begin();
  Wire.setClock(100000);   // bring-up at 100 kHz; later you can go 400 kHz

  Serial.println(mAddr, HEX);
  // You can pass &Wire explicitly if you prefer.
  mOk = mImu.begin_I2C(mAddr, &Wire);
  if (!mOk) return false;

  // Reasonable defaults
  mImu.setAccelRange(LSM6DS_ACCEL_RANGE_4_G);
  mImu.setGyroRange(LSM6DS_GYRO_RANGE_500_DPS);
  mImu.setAccelDataRate(LSM6DS_RATE_104_HZ);
  mImu.setGyroDataRate(LSM6DS_RATE_104_HZ);

  // initialize cached values
  update(true);
  return true;
}

bool ImuSensor::update(bool force)
{
  if (!mOk) return false;

  uint32_t now = millis();
  if (!force && (now - mLastReadMs) < mMinIntervalMs) return false;
  mLastReadMs = now;

  sensors_event_t accel, gyro, temp;
  mImu.getEvent(&accel, &gyro, &temp);

  // Adafruit gives accel in m/s^2, gyro in rad/s
  mAx = accel.acceleration.x;
  mAy = accel.acceleration.y;
  mAz = accel.acceleration.z;

  mGx = gyro.gyro.x;
  mGy = gyro.gyro.y;
  mGz = gyro.gyro.z;

  return true;
}

float ImuSensor::getAx() { return mAx; }
float ImuSensor::getAy() { return mAy; }
float ImuSensor::getAz() { return mAz; }
float ImuSensor::getGx() { return mGx; }
float ImuSensor::getGy() { return mGy; }
float ImuSensor::getGz() { return mGz; }

float ImuSensor::getAccelNorm()
{
  if (!isfinite(mAx) || !isfinite(mAy) || !isfinite(mAz)) return NAN;
  return sqrtf(mAx*mAx + mAy*mAy + mAz*mAz);
}

float ImuSensor::getGyroNorm()
{
  if (!isfinite(mGx) || !isfinite(mGy) || !isfinite(mGz)) return NAN;
  return sqrtf(mGx*mGx + mGy*mGy + mGz*mGz);
}

String ImuSensor::packCSV(uint8_t decimals)
{
  update(false);
  // Example: "Ax:0.012,Ay:...,Az:...,Gx:...,Gy:...,Gz:..."
  String s;
  s.reserve(96);
  s += "Ax:"; s += String(mAx, decimals);
  s += ",Ay:"; s += String(mAy, decimals);
  s += ",Az:"; s += String(mAz, decimals);
  s += ",Gx:"; s += String(mGx, decimals);
  s += ",Gy:"; s += String(mGy, decimals);
  s += ",Gz:"; s += String(mGz, decimals);
  return s;
}
