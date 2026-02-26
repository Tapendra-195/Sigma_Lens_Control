#include "../include/CameraFirmware.h"
#include "../include/Config.h"

RingBuffer<EVENT, 20> CameraFirmware::mInputBuffer;

// Returns heading in degrees [0, 360)
static float heading_deg_tilt_comp(float ax, float ay, float az,
                                   float mx, float my, float mz)
{
  // Normalize accel (avoid divide-by-zero)
  const float an = sqrtf(ax*ax + ay*ay + az*az);
  if (an < 1e-6f) return NAN;
  ax /= an; ay /= an; az /= an;

  // Roll (phi) and pitch (theta)
  // Assumes +Z up-ish; works fine for small/medium tilts
  const float roll  = atan2f(ay, az);
  const float pitch = atanf(-ax / sqrtf(ay*ay + az*az));

  // Tilt-compensate mag
  const float cr = cosf(roll),  sr = sinf(roll);
  const float cp = cosf(pitch), sp = sinf(pitch);

  // Rotate mag by pitch then roll (common convention)
  const float mx2 = mx*cp + mz*sp;
  const float my2 = mx*sr*sp + my*cr - mz*sr*cp;
  // mz2 not needed for heading

  float hdg = atan2f(-my2, mx2) * 180.0f / (float)M_PI; // sign may need flip
  if (hdg < 0) hdg += 360.0f;
  return hdg;
}


CameraFirmware::CameraFirmware()
{
  pinMode(static_cast<uint8_t>(LENS_PIN::LENS_PWR_SW), OUTPUT);
  pinMode(static_cast<uint8_t>(LENS_PIN::LOGIC_VCC_SW), OUTPUT);    
  pinMode(static_cast<uint8_t>(LENS_PIN::LENS_DETECT), INPUT_PULLUP);

  reset();
  
  //Initialize start state to idle
  //Could make Off the start state, If we want it to be off by default and turn on using the front end.
  mState = &LensState::idle;
  mState->enter(*this);

}

void CameraFirmware::updateFSM()
{
  EVENT input;
  //process input if there is any
  if(mInputBuffer.pop(input))
    {
      mState->handleInput(*this, input);
    }
  mState->update(*this);
}

void CameraFirmware::attachHumiditySensor(HumiditySensor* s)
{
  humiditySensor = s;
  if (!humiditySensor) return;

  // Ensure sensor rail is powered even with no lens
  pinMode(static_cast<uint8_t>(LENS_PIN::LOGIC_VCC_SW), OUTPUT);
  digitalWrite(static_cast<uint8_t>(LENS_PIN::LOGIC_VCC_SW), HIGH);

  delay(20); // allow rail + BME280 to come up
  humiditySensor->begin();   // now it will power-on + init reliably

}

void CameraFirmware::attachImuSensor(ImuSensor* s)
{
  imuSensor = s;
  if (!imuSensor) return;

  // Ensure sensor rail is powered even with no lens
  pinMode(static_cast<uint8_t>(LENS_PIN::LOGIC_VCC_SW), OUTPUT);
  digitalWrite(static_cast<uint8_t>(LENS_PIN::LOGIC_VCC_SW), HIGH);

  delay(20); // allow rail + IMU to come up
  bool ok = imuSensor->begin();   // now it will power-on + init reliably
  Serial.printf("LSM6DS3 begin @0x6A -> %s\n", ok ? "OK" : "FAIL");
} 

void CameraFirmware::attachMagSensor(MagSensor* s)
{
  magSensor = s;
  if (!magSensor) return;

  // Ensure sensor rail is powered even with no lens
  pinMode(static_cast<uint8_t>(LENS_PIN::LOGIC_VCC_SW), OUTPUT);
  digitalWrite(static_cast<uint8_t>(LENS_PIN::LOGIC_VCC_SW), HIGH);

  delay(20); // allow rail + Mag sensor to come up
  bool ok = magSensor->begin();   // now it will power-on + init reliably
  Serial.printf("LIS3MDL begin @0x1E -> %s\n", ok ? "OK" : "FAIL");

}

void CameraFirmware::handleLensDetection()
{
  bool isDetected = !digitalRead(static_cast<uint8_t>(LENS_PIN::LENS_DETECT));
  
  if(!lensStatus.isConnected & isDetected)
    {
      mInputBuffer.push(EVENT::LENS_CONNECTED);
      lensStatus.isConnected = true;
    }
  else if(lensStatus.isConnected & !isDetected)
    {
      mInputBuffer.push(EVENT::LENS_DISCONNECTED);
      lensStatus.isConnected = false;
    }

}

void CameraFirmware::reset()
{
  lensStatus = LensStatus{0x00, 0x00, 0x00, "", "", false};
  disablePolling();
}

void CameraFirmware::enablePolling()
{
  mPollLens = true;
}

void CameraFirmware::disablePolling()
{
  mPollLens = false;
}

void CameraFirmware::run()
{
  //Process bytes coming from the lens
  //processByte is non blocking. It strings bytes into a message.
  processByte();
  handleFrontEndInput();  
  handleLensDetection();
  updateFSM();
  
  bool bcalib_loop = false;
  if (bcalib_loop) {
    static float xmin=1e9, xmax=-1e9;
    static float ymin=1e9, ymax=-1e9;
    static float zmin=1e9, zmax=-1e9;

    // ===== MAG CAL MODE =====
    static uint32_t lastPrintUs = 0;
    const uint32_t periodUs = 5000;  // 200 Hz (adjust if needed)

    if (micros() - lastPrintUs >= periodUs) {
      lastPrintUs += periodUs;

      if (magSensor) {
        magSensor->update(true);  // force read

        float bx = magSensor->getBx_uT();
        float by = magSensor->getBy_uT();
        float bz = magSensor->getBz_uT();

        // FAST CSV (no labels, minimal overhead)
        //Serial.print(bx, 3); Serial.print(",");
        //Serial.print(by, 3); Serial.print(",");
        //Serial.println(bz, 3);

        xmin = min(xmin, bx); xmax = max(xmax, bx);
        ymin = min(ymin, by); ymax = max(ymax, by);
        zmin = min(zmin, bz); zmax = max(zmax, bz);
        static uint32_t lastStatsMs = 0;
        if (millis() - lastStatsMs > 2000) {
          lastStatsMs = millis();

          Serial.print("# min/max: ");
          Serial.print(xmin); Serial.print(","); Serial.print(xmax); Serial.print("  ");
          Serial.print(ymin); Serial.print(","); Serial.print(ymax); Serial.print("  ");
          Serial.print(zmin); Serial.print(","); Serial.println(zmax);
        }

      }
    }
  }

  // --- Status heartbeat (independent of lens/FSM) ---
  const unsigned long nowMs = millis();
  if (nowMs - lastStatusMs >= statusPeriodMs) {
    lastStatusMs = nowMs;

    lensStatus.extra = "";

    if (humiditySensor != nullptr) {
      lensStatus.extra += humiditySensor->packCSV(1);  // H/T/P
    }

    if (imuSensor != nullptr) {
      if (lensStatus.extra.length() > 0) lensStatus.extra += ",";
      lensStatus.extra += imuSensor->packCSV(3);      // Ax..Gz
    }

    if (magSensor != nullptr) {
      if (lensStatus.extra.length() > 0) lensStatus.extra += ",";
      lensStatus.extra += magSensor->packCSV(2);      // Bx..Bz
    }

    if (magSensor != nullptr && imuSensor != nullptr) {
      float ax = imuSensor->getAx();
      float ay = imuSensor->getAy();
      float az = imuSensor->getAz();

      float bx = magSensor->getBx_uT();
      float by = magSensor->getBy_uT();
      float bz = magSensor->getBz_uT();

      float hdg = heading_deg_tilt_comp(ax, ay, az, bx, by, bz);

      if (!isnan(hdg)) {
        if (lensStatus.extra.length() > 0) lensStatus.extra += ",";
        lensStatus.extra += "Hd:";
        lensStatus.extra += String(hdg, 1);
      }
    }

    Serial.println(getStatus());
  }

  
  //Poll Lens
  unsigned long currentTime = micros();
  // Check if it's time to issue the low pulse:
  if (currentTime - lastPulseTime >= totalPeriod) {
    //Send Message back to the front end
    //Serial.println(getStatus());
    if(mPollLens)
      {
	      pollLens();
      }
    // Reset timer
    lastPulseTime = currentTime;
  }
}


void CameraFirmware::handleFrontEndInput()
{
  if (Serial.available()) {
      String message = Serial.readStringUntil('\n');
      message.trim();

      int spaceIndex = message.indexOf(' ');
      String cmd, arg;
      
      if (spaceIndex == -1) {
        cmd = message; // Only one word
      } else {
        cmd = message.substring(0, spaceIndex);
        arg = message.substring(spaceIndex + 1);

      }
      
      if (cmd == "SA") { // Set Aperture
        unsigned int aperture = (unsigned int)arg.toInt();
	      mMessage03.setAperture(aperture);
      } 
      else if (cmd == "SF") { // Set Focus
	      unsigned int targetLensPos = (unsigned int)arg.toInt();
	      mMessage04.setLensPos(lensStatus.currentLensPos, targetLensPos);
      } 
      else if (cmd == "ON") {
	      mInputBuffer.push(EVENT::POWER_ON);
      } 
      else if (cmd == "OFF") {
	      mInputBuffer.push(EVENT::POWER_OFF);
      }
      else {
	//Ignore unknown command
      }
  }
}

//Sends a polling signal to lens to retrive the current focus and aperture of the lens.
void CameraFirmware::pollLens()
{
    digitalWrite(uint8_t(LENS_PIN::BODY_POLL_LENS), LOW);
    delayMicroseconds(lowDuration);
    digitalWrite(uint8_t(LENS_PIN::BODY_POLL_LENS), HIGH);
}

void CameraFirmware::sendMessage(const byte* message, int length)
{
  //wait till LENS_CS_BODY goes low
  while (digitalRead(static_cast<uint8_t>(LENS_PIN::LENS_CS_BODY))) {
    delayMicroseconds(1);
  }
  digitalWrite(static_cast<uint8_t>(LENS_PIN::BODY_CS_LENS), HIGH);
  delayMicroseconds(40);  
    
  Serial1.write(message, length); //send message
    
  Serial1.flush();
  delayMicroseconds(40);
  digitalWrite(static_cast<uint8_t>(LENS_PIN::BODY_CS_LENS), LOW);
}

String CameraFirmware::getStatus()
{
  return "ID:"+String(cameraID)+",F:"+String(lensStatus.currentLensPos) + ",A:" + String(lensStatus.currentAperture)+",S:"+String(lensStatus.currentState)+","+lensStatus.extra;
}


void CameraFirmware::processByte()
{
  if(!(Serial1.available()>0))
    {
      //There is no message in buffer. Return.
      return;
    }
  
  int read = Serial1.read();
  if (lensToBodyBufferPosition == INVALID_POSITION) 
    {
      if (static_cast<byte>(BYTE_VALUE::SOM))
	{
	  lensToBodyBufferPosition = 0;
        }
      else
	{
	  return;
	}
    }
  
  lensToBodyBuffer[lensToBodyBufferPosition++] = read;
  if (lensToBodyBufferPosition >= MAX_BUFFER_SIZE)
    {
      //ERROR
      lensToBodyBufferPosition = INVALID_POSITION;
    }
  else if (lensToBodyBufferPosition == 2)
    {
      packetLength = (lensToBodyBuffer[2] << 8) + lensToBodyBuffer[1];
    }
  if (lensToBodyBufferPosition == packetLength)
    {
      if (read == static_cast<byte>(BYTE_VALUE::EOM))
	{
	  mInputBuffer.push(EVENT::PROCESS_MESSAGE);
	  
	  lensToBodyBufferPosition = INVALID_POSITION;
	}
      else
	{
	  //Error
	  lensToBodyBufferPosition = INVALID_POSITION;
	}
    }
}


