#include "../include/HumiditySensor.h"

#include <Wire.h>

// Adafruit BME280 library
#include <Adafruit_BME280.h>

// One global instance for this translation unit
static Adafruit_BME280 gBme;

namespace
{
  // One sensor instance per translation unit. If you ever need multiple sensors,
  // move this into the class as a member and store by value.
  Adafruit_BME280 g_bme;
}

HumiditySensor::HumiditySensor(uint8_t i2cAddress)
  : mAddr(i2cAddress)
{
}

void HumiditySensor::setPowerOnHook(std::function<void(void)> fn)
{
  mPowerOnHook = fn;
}

bool HumiditySensor::begin()
{

  if (mPowerOnHook) {
    mPowerOnHook();          // ensure TPS22918 EN is high
    delay(20);               // allow rail + BME280 to come up
  } else {
    Serial.println("BME280: no power hook");
  }

  Wire.begin();

  Serial.println(mAddr, HEX);

  mOk = gBme.begin(mAddr, &Wire);

  gBme.setSampling(
    Adafruit_BME280::MODE_SLEEP,   // start sleeping
    Adafruit_BME280::SAMPLING_X1,  // temp oversampling
    Adafruit_BME280::SAMPLING_X1,  // pressure oversampling
    Adafruit_BME280::SAMPLING_X1,  // humidity oversampling
    Adafruit_BME280::FILTER_OFF
  );

  if (!mOk) {
    mHumidity = NAN;
    mTempC = NAN;
    mPressurehPa = NAN;
    return false;
  }

  // Prime the cache.
  mLastReadMs = 0;
  update(true);

  return true;
}


bool HumiditySensor::update(bool force)
{
  if (!mOk) {
    return false;
  }

  unsigned long now = millis();
  if (!force && (now - mLastReadMs) < 10000) { // only poll every 10s
    return true; // keep last cached values
  }

  gBme.setSampling(
    Adafruit_BME280::MODE_FORCED,
    Adafruit_BME280::SAMPLING_X1,
    Adafruit_BME280::SAMPLING_X1,
    Adafruit_BME280::SAMPLING_X1,
    Adafruit_BME280::FILTER_OFF
  );

  // Wait for measurement to complete (a few ms at X1)
  delay(10);

  // Always read when called (simple + deterministic)
  float t = gBme.readTemperature();
  float p = gBme.readPressure() / 100.0f; // Pa -> hPa
  float h = gBme.readHumidity();

  mTempC = t;
  mHumidity = h;
  mPressurehPa = p;
  mLastReadMs = millis();




  return true;
}

float HumiditySensor::getHumidity()
{
  update(false);
  return mHumidity;
}

float HumiditySensor::getTemperatureC()
{
  update(false);
  return mTempC;
}

float HumiditySensor::getPressurehPa()
{
  update(false);
  return mPressurehPa;
}

String HumiditySensor::packCSV(uint8_t decimals)
{
  update(false);
  if (!mOk || isnan(mHumidity) || isnan(mTempC) || isnan(mPressurehPa))
    {
      return String("H:nan,T:nan,P:nan");
    }

  // Use String(float, decimals) for formatting.
  // Keep keys short for easy parsing on the front end.
  String s;
  s.reserve(32);
  s += "H:";
  s += String(mHumidity, decimals);
  s += ",T:";
  s += String(mTempC, decimals);
  s += ",P:";
  s += String(mPressurehPa, decimals);
  return s;
}
