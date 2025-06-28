#include "../include/InitializationState.h"
#include "../include/CameraFirmware.h"

void InitializationState::enter(CameraFirmware& firmware)
{
  //Does the handshake, expects ACK from lens. We're ignoring that. We'll send the pulse, but won't check if there is ACK.
  //Don't want to add more states.
  delayMicroseconds(6);
  digitalWrite(static_cast<uint8_t>(LENS_PIN::BODY_CS_LENS), HIGH);
  delayMicroseconds(1088);
  digitalWrite(static_cast<uint8_t>(LENS_PIN::BODY_CS_LENS), LOW);
  
  Serial1.begin(1500000, SERIAL_8N1); //change baud rate after speed negotiation
  
  //Power Lens motors.
  digitalWrite(static_cast<uint8_t>(LENS_PIN::LENS_PWR_SW), HIGH);
  
  delayMicroseconds(1000);
  firmware.lensStatus.currentState = "INITIALIZATION";
  //First initialization message
  firmware.sendMessage(init0B, sizeof(init0B));
}

void InitializationState::handleInput(CameraFirmware& firmware, EVENT e)
{
  if(e == EVENT::PROCESS_MESSAGE)
    {
      Message msg(firmware.lensToBodyBuffer);
      switch (msg.getMessageType())
	{
	case 0x0B:
	  firmware.sendMessage(init3F, sizeof(init3F));
	  break;
	case 0x3F:
	  firmware.sendMessage(init3D, sizeof(init3D));
	  break;
	case 0x3D:
	  firmware.sendMessage(init08, sizeof(init08));
	  break;
	case 0x08:
	  firmware.sendMessage(init09, sizeof(init09));
	  break;
	case 0x09:
	  firmware.sendMessage(init0D, sizeof(init0D));
	  break;
	case 0x0D:
	  firmware.sendMessage(init10, sizeof(init10));
	  break;
	case 0x10:
	  firmware.sendMessage(init0A, sizeof(init0A));
	  break;
	case 0x0A:
	  //Initialization is complete.
	  firmware.mState = &LensState::regularMessaging;
	  firmware.mState->enter(firmware);
	  break;        
	default:
	  //Unknown message received. Reset.
	  firmware.mState = &LensState::idle;
	  firmware.mState->enter(firmware);
	  firmware.mResetCount++;
	  break;
	}
    }
  else if(e == EVENT::POWER_OFF)
    {
      //GO TO Shut down
      firmware.mState = &LensState::shutDown;
      firmware.mState->enter(firmware);
      
    }
  else if(e == EVENT::LENS_DISCONNECTED)
    {
      //GO TO OFF
      firmware.mState = &LensState::off;
      firmware.mState->enter(firmware);
    }
}
