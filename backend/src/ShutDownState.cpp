#include "../include/ShutDownState.h"
#include "../include/CameraFirmware.h"

void ShutDownState::enter(CameraFirmware& firmware)
{
  firmware.sendMessage(off0A, sizeof(off0A)); //Changes Aperture to some const value.
  firmware.lensStatus.currentState = "ShutDown";
}

void ShutDownState::handleInput(CameraFirmware& firmware, EVENT e)
{
  if(e == EVENT::PROCESS_MESSAGE)
    {
      Message msg(firmware.lensToBodyBuffer);
      switch(msg.getMessageType())
	{
	case 0x0A:
	  firmware.sendMessage(off16, sizeof(off16));
	  break;
	case 0x16:
	  //power off power to lens
	  firmware.mState = &LensState::off;
	  firmware.mState->enter(firmware);
	  break;
	case 0x05:
	  //ignore
	  break;
	case 0x06:
	  //ignore
	  break;
	default:
	  firmware.mState = &LensState::off;
	  firmware.mState->enter(firmware);
	  break;
	}
    }
  
  else if(e == EVENT::POWER_OFF)
    {
      //ignore
    }
  else if(e == EVENT::LENS_DISCONNECTED)
    {
      //GO TO OFF
      firmware.mState = &LensState::off;
      firmware.mState->enter(firmware);
    }
}
