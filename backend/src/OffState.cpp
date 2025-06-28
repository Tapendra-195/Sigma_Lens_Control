#include "../include/OffState.h"
#include "../include/CameraFirmware.h"

void OffState::enter(CameraFirmware& firmware)
{
  Serial1.end();
  //Make all pins High Impedence, except the switch pins. 
  pinMode(static_cast<uint8_t>(LENS_PIN::TX), INPUT);
  pinMode(static_cast<uint8_t>(LENS_PIN::RX), INPUT);
  pinMode(static_cast<uint8_t>(LENS_PIN::BODY_POLL_LENS), INPUT);
  pinMode(static_cast<uint8_t>(LENS_PIN::BODY_CS_LENS), INPUT);
  
  //Turn off Logic VCC and Lens PWr.
  digitalWrite(static_cast<uint8_t>(LENS_PIN::LOGIC_VCC_SW), LOW);
  digitalWrite(static_cast<uint8_t>(LENS_PIN::LENS_PWR_SW), LOW);
  
  //Disable lens polling
  firmware.mPollLens = true;
  firmware.lensStatus.currentState = "OFF";
}

void OffState::handleInput(CameraFirmware& firmware, EVENT e)
{
  if(e == EVENT::POWER_ON)
    {
      //IDLE
      firmware.mState = &LensState::idle;
      firmware.mState->enter(firmware);
    }
}
