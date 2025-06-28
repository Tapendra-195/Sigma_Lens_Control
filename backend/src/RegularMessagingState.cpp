#include "../include/RegularMessagingState.h"
#include "../include/CameraFirmware.h"

void RegularMessagingState::enter(CameraFirmware& firmware)
{
  //Start with default value. 
  firmware.mMessage03.reset();
  firmware.mMessage04.reset();
  
  //Start Polling lens
  firmware.mPollLens = true;
  firmware.lensStatus.currentState = "RegularMessaging";
}

void RegularMessagingState::handleInput(CameraFirmware& firmware, EVENT e)
{
  if(e == EVENT::PROCESS_MESSAGE)
    {
      Message msgTemp(firmware.lensToBodyBuffer);
      Message* msg = &msgTemp;
      switch (msg->getMessageType())
	{
	case 0x05:
	  {
	    firmware.lensStatus.currentAperture = static_cast<Message05*>(msg)->getAperture(); //Not a problem because 05 class doesn't contain any more data than the base class.
	    uint16_t currentApertureDialValue = static_cast<Message05*>(msg)->getApertureDialValue();
	    //If Aperture Dial is Rotated, change Aperture value based on the aperture dial value.
	    if(firmware.lensStatus.apertureDialValue != currentApertureDialValue)
	      {
		firmware.lensStatus.apertureDialValue = currentApertureDialValue;
		firmware.mMessage03.setAperture(currentApertureDialValue);
	      }
	  }
	  break;
	case 0x06:
	  {
	    firmware.lensStatus.currentLensPos = static_cast<Message06*>(msg)->getLensPos();
	    
	    
	    firmware.mMessage03.update();
	    firmware.mMessage03.prepForSending();
	    firmware.sendMessage(firmware.mMessage03.mMessageBuffer, firmware.mMessage03.getMessageLength());
	    delayMicroseconds(10);
	    
	    firmware.mMessage04.update();
	    firmware.mMessage04.prepForSending();
	    firmware.sendMessage(firmware.mMessage04.mMessageBuffer, firmware.mMessage04.getMessageLength());
	  }
	  break;
	  
	default:
	  //In regular messaging we should only get 0x05 and 0x06 type message from the lens. Any other message represents error. Therefore reset.
	  firmware.mState = &LensState::idle;
	  firmware.mState->enter(firmware);
	  firmware.mResetCount++;
	  break;
	}
      }
  
  else if(e == EVENT::POWER_OFF)
    {
      //GO TO POWERING OFF
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
