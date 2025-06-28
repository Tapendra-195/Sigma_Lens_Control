#include "../include/LinkEstablishmentState.h"
#include "../include/CameraFirmware.h"

void LinkEstablishmentState::enter(CameraFirmware& firmware)
{
    startTime = micros();
  
    pinMode(static_cast<uint8_t>(LENS_PIN::TX), OUTPUT); //UART hasn't started yet. Still doing link establishment.
    pinMode(static_cast<uint8_t>(LENS_PIN::RX), INPUT);
  
    pinMode(static_cast<uint8_t>(LENS_PIN::BODY_POLL_LENS), OUTPUT);
    pinMode(static_cast<uint8_t>(LENS_PIN::BODY_CS_LENS), OUTPUT);
    
    //Start with all output pins low
    digitalWrite(static_cast<uint8_t>(LENS_PIN::BODY_POLL_LENS), LOW);
    digitalWrite(static_cast<uint8_t>(LENS_PIN::BODY_CS_LENS), LOW);
    digitalWrite(static_cast<uint8_t>(LENS_PIN::TX), LOW);
    digitalWrite(static_cast<uint8_t>(LENS_PIN::LOGIC_VCC_SW), LOW);
    
  //power logic vcc
    digitalWrite(static_cast<uint8_t>(LENS_PIN::LOGIC_VCC_SW), HIGH);
    //set polling to default high
    digitalWrite(static_cast<uint8_t>(LENS_PIN::BODY_POLL_LENS), HIGH);
    
    //Lens is detected, now pull BODY_CS_LENS HIGH to start handshake.
    digitalWrite(static_cast<uint8_t>(LENS_PIN::BODY_CS_LENS), HIGH);
    mPhase = PHASE::WAIT_FOR_CS_HIGH;
    firmware.lensStatus.currentState = "LinkEstablishment";
}

STATUS LinkEstablishmentState::pollPhase()
{
  switch (mPhase)
    {
    case PHASE::WAIT_FOR_CS_HIGH:
	if (digitalRead(static_cast<uint8_t>(LENS_PIN::LENS_CS_BODY))) {
	    digitalWrite(static_cast<uint8_t>(LENS_PIN::TX), HIGH);
	    mPhase = PHASE::WAIT_FOR_RX_HIGH;
	}
	return STATUS::IN_PROGRESS;
	
    case PHASE::WAIT_FOR_RX_HIGH:
	if (digitalRead(static_cast<uint8_t>(LENS_PIN::RX))) {
	    digitalWrite(static_cast<uint8_t>(LENS_PIN::BODY_CS_LENS), LOW);
	    mPhase = PHASE::WAIT_FOR_CS_LOW;
	}
	return STATUS::IN_PROGRESS;
	
    case PHASE::WAIT_FOR_CS_LOW:
	if (!digitalRead(static_cast<uint8_t>(LENS_PIN::LENS_CS_BODY))) {
	    mPhase = PHASE::COMPLETE;
	    return STATUS::SUCCESS;
	}
	return STATUS::IN_PROGRESS;
	
    case PHASE::COMPLETE:
	return STATUS::SUCCESS;
    }
  
  return STATUS::FALIURE; // should never reach
}

void LinkEstablishmentState::handleInput(CameraFirmware& firmware, EVENT e)
{
    if(e == EVENT::POWER_OFF)
    {
	firmware.mState = &LensState::off;
	firmware.mState->enter(firmware);
    }
    else if(e == EVENT::LENS_DISCONNECTED)
    {
	firmware.mState = &LensState::off;
	firmware.mState->enter(firmware);
    }
}

void LinkEstablishmentState::update(CameraFirmware& firmware) 
{
    unsigned long delTime = micros() - startTime;
    if(delTime > 2000000) //delTime>2 sec
    {
	firmware.mState = &LensState::idle;
	firmware.mState->enter(firmware);
	firmware.mResetCount++;

	return;
    }
    
    STATUS result = pollPhase();
    if(result == STATUS::SUCCESS)
    {
	firmware.mState = &LensState::speedNegotiation;
	firmware.mState->enter(firmware);
      }
}
