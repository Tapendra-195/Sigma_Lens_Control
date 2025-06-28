#include "../include/CameraFirmware.h"
#include "../include/Config.h"

RingBuffer<EVENT, 20> CameraFirmware::mInputBuffer;

CameraFirmware::CameraFirmware()
{
  pinMode(static_cast<uint8_t>(LENS_PIN::LENS_PWR_SW), OUTPUT);
  pinMode(static_cast<uint8_t>(LENS_PIN::LOGIC_VCC_SW), OUTPUT);    
  pinMode(static_cast<uint8_t>(LENS_PIN::LENS_DETECT), INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(static_cast<uint8_t>(LENS_PIN::LENS_DETECT)), lensDetectISR, CHANGE);

    //Initialize start state to idle
    //Could make Off the start state, If we want it to be off by default and turn on using the front end.
  mState = &LensState::idle;
  mState->enter(*this);

  Serial.begin(115200); //For usb communication with front end.
}

void CameraFirmware::lensDetectISR()
{
  if (digitalRead(static_cast<uint8_t>(LENS_PIN::LENS_DETECT)) == LOW) {
    mInputBuffer.push(EVENT::LENS_CONNECTED);
  } else {
    mInputBuffer.push(EVENT::LENS_DISCONNECTED);
  }
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


void CameraFirmware::run()
{
  //Process bytes coming from the lens
  //processByte is non blocking. It strings bytes into a message.
  processByte();
  handleFrontEndInput();  
  updateFSM();
  
  
  //Poll Lens
  unsigned long currentTime = micros();
  // Check if it's time to issue the low pulse:
  if (currentTime - lastPulseTime >= totalPeriod) {
    //Send Message back to the front end
    if(humiditySensor != nullptr)
      {
	lensStatus.extra = "H:" + String(humiditySensor->getHumidity());
      }
    Serial.println(getStatus());
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
