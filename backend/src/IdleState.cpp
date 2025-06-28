#include "../include/IdleState.h"
#include "../include/CameraFirmware.h"

void IdleState::enter(CameraFirmware& firmware)
{
  Serial1.end();
  //Make all pins High Impedence, except the switch pins. 
  pinMode(static_cast<uint8_t>(LENS_PIN::BODY_POLL_LENS), INPUT);
  pinMode(static_cast<uint8_t>(LENS_PIN::LENS_CS_BODY), INPUT);
  pinMode(static_cast<uint8_t>(LENS_PIN::RX), INPUT);
  pinMode(static_cast<uint8_t>(LENS_PIN::TX), INPUT);
  pinMode(static_cast<uint8_t>(LENS_PIN::BODY_CS_LENS), INPUT);
  
  //Turn the Logic Vcc and Lens Pwr off using a transistor in the beginning.
  digitalWrite(static_cast<uint8_t>(LENS_PIN::LOGIC_VCC_SW), LOW);
  digitalWrite(static_cast<uint8_t>(LENS_PIN::LENS_PWR_SW), LOW);
  
  firmware.mPollLens = false;
  firmware.lensStatus.currentState = "IDLE";
  delayMicroseconds(500);//just a random delay.
}


void IdleState::handleInput(CameraFirmware& firmware, EVENT e)
{
  if(firmware.mResetCount > 4)
    {
      firmware.mState = &LensState::off;
      firmware.mState->enter(firmware);
      
      return;
    }
  
  if(e == EVENT::LENS_CONNECTED)
    {
      firmware.mState = &LensState::linkEstablishment;
      firmware.mState->enter(firmware);
    }
}
