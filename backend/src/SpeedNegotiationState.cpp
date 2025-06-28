#include "../include/SpeedNegotiationState.h"
#include "../include/CameraFirmware.h"

void SpeedNegotiationState::enter(CameraFirmware& firmware)
{
  Serial1.setTX(static_cast<uint8_t>(LENS_PIN::TX));
  Serial1.setRX(static_cast<uint8_t>(LENS_PIN::RX));
  Serial1.begin(750000, SERIAL_8N1); //Starts with 750K baud rate
  
  delayMicroseconds(500);//just a random delay.
  
  firmware.lensStatus.currentState = "SpeedNegotiation";
  
  //First msg for speed Negotiation
  firmware.sendMessage(init01, sizeof(init01));
}

void SpeedNegotiationState::handleInput(CameraFirmware& firmware, EVENT e)
{
  if(e == EVENT::PROCESS_MESSAGE)
    {
      Message msg(firmware.lensToBodyBuffer);
      switch (msg.getMessageType())
	{
	case 0x01:
	  firmware.sendMessage(init07, sizeof(init07));
	  break;
	case 0x07:
	  firmware.sendMessage(init0C, sizeof(init0C));
	  break;
	case 0x0C:
	  //Speed Negotiation complete
	  firmware.mState = &LensState::initialization;
	  firmware.mState->enter(firmware);
	  break;
	default:
	  //unknown message received, reset
	  firmware.mState = &LensState::idle;
	  firmware.mState->enter(firmware);
	  firmware.mResetCount++;
	  break;
	}
    }
  else if(e == EVENT::POWER_OFF)
    {
      firmware.mState = &LensState::shutDown;
      firmware.mState->enter(firmware);
    }
  else if(e == EVENT::LENS_DISCONNECTED)
    {
      firmware.mState = &LensState::off;
      firmware.mState->enter(firmware);
    }
}
