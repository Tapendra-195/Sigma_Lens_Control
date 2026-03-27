#include "../include/OffState.h"
#include "../include/CameraFirmware.h"

void OffState::enter(CameraFirmware& firmware)
{
  Serial.println("DBG: ENTER Off");
  Serial1.end();
  //Make all pins High Impedence, except the switch pins. 
  pinMode(static_cast<uint8_t>(LENS_PIN::TX), INPUT);
  pinMode(static_cast<uint8_t>(LENS_PIN::RX), INPUT);
  pinMode(static_cast<uint8_t>(LENS_PIN::BODY_POLL_LENS), INPUT);
  pinMode(static_cast<uint8_t>(LENS_PIN::BODY_CS_LENS), INPUT);
  
  // Keep shared logic rail on for sensors; only shut lens power off.
  digitalWrite(static_cast<uint8_t>(LENS_PIN::LOGIC_VCC_SW), HIGH);
  digitalWrite(static_cast<uint8_t>(LENS_PIN::LENS_PWR_SW), LOW);
  
  //Disable lens polling
  firmware.disablePolling();
  firmware.lensStatus.currentState = "Off";
}

void OffState::handleInput(CameraFirmware& firmware, EVENT e)
{
  Serial.printf("DBG: OffState::handleInput e=%d\n", (int)e);
  if(e == EVENT::POWER_ON)
    {
      Serial.println("DBG: OffState handling POWER_ON -> Idle");

      //IDLE
      firmware.mResetCount = 0;
      firmware.mState = &LensState::idle;
      firmware.mState->enter(firmware);
    }
}
