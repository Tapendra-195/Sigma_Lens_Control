#include "../include/MagSensor.h"
#include "../include/Constants.h"
#include <math.h>

MagSensor::MagSensor(uint8_t i2cAddress) : mAddr(i2cAddress) {}

void MagSensor::setPowerOnHook(std::function<void(void)> fn) { mPowerOnHook = fn; }

//void MagSensor::setOffset_uT(float ox, float oy, float oz)
//{
//  mOffx = ox; mOffy = oy; mOffz = oz;
//}
static uint8_t readReg(uint8_t addr, uint8_t reg)
{
  Wire.beginTransmission(addr);
  Wire.write(reg);
  uint8_t err = Wire.endTransmission(false);
  if (err != 0) return 0xFF;

  Wire.requestFrom(addr, (uint8_t)1);
  return Wire.available() ? Wire.read() : 0xFF;
}

static void dumpImuId()
{
  uint8_t who = readReg(0x6A, 0x0F);
  Serial.printf("IMU WHO_AM_I (0x0F) @0x6A = 0x%02X\n", who);
}

bool MagSensor::begin()
{
  if (mPowerOnHook) {
    mPowerOnHook();          // ensure TPS22918 EN is high
    delay(20);               // allow rail + BME280 to come up
  } else {
    Serial.println("MAG: no power hook");
  }

  Wire.begin();
  Wire.setClock(100000);   // bring-up at 100 kHz; later you can go 400 kHz

  Serial.println(mAddr, HEX);

  dumpImuId();
  
  mOk = mMag.begin_I2C(mAddr, &Wire);
  if (!mOk) return false;

  // Conservative defaults
  mMag.setPerformanceMode(LIS3MDL_MEDIUMMODE);
  mMag.setOperationMode(LIS3MDL_CONTINUOUSMODE);
  mMag.setDataRate(LIS3MDL_DATARATE_155_HZ);
  mMag.setRange(LIS3MDL_RANGE_4_GAUSS);

  // initialize cached values
  update(true);

  return true;
}

bool MagSensor::update(bool force)
{
  if (!mOk) return false;

  uint32_t now = millis();
  if (!force && (now - mLastReadMs) < mMinIntervalMs) return false;
  mLastReadMs = now;

  sensors_event_t event;
  mMag.getEvent(&event);

  // Adafruit returns microtesla
  mBx = event.magnetic.x ;
  mBy = event.magnetic.y ;
  mBz = event.magnetic.z ;


  // apply calibration offsets (if set)
  // hard-iron
  float bx = mBx - MAG_OX;
  float by = mBy - MAG_OY;
  float bz = mBz - MAG_OZ;

  // simple soft-iron (diagonal)
  bx *= MAG_SX;
  by *= MAG_SY;
  bz *= MAG_SZ;

  mBx = bx;
  mBy = by;
  mBz = bz;

  return true;
}

float MagSensor::getBx_uT() { return mBx; }
float MagSensor::getBy_uT() { return mBy; }
float MagSensor::getBz_uT() { return mBz; }

float MagSensor::getBnorm_uT()
{
  if (!isfinite(mBx) || !isfinite(mBy) || !isfinite(mBz)) return NAN;
  return sqrtf(mBx*mBx + mBy*mBy + mBz*mBz);
}

String MagSensor::packCSV(uint8_t decimals)
{
  update(false);
  // Example: "Bx:12.3,By:-4.5,Bz:51.2"
  String s;
  s.reserve(64);
  s += "Bx:"; s += String(mBx, decimals);
  s += ",By:"; s += String(mBy, decimals);
  s += ",Bz:"; s += String(mBz, decimals);
  return s;
}
