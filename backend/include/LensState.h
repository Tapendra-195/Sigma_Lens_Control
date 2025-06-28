#pragma once
#include "Arduino.h"
#include "Message.h"                                                 
#include "Message03.h"                                               
#include "Message04.h"                                               
#include "Message05.h"                                               
#include "Message06.h"

enum class EVENT
  {
    LENS_CONNECTED,
    LENS_DISCONNECTED,
    PROCESS_MESSAGE,
    POWER_OFF,
    POWER_ON
  };

//Teensy pin the lens pins are connected to
enum class LENS_PIN
{
    RX = 0,
    TX = 1,
    LENS_CS_BODY = 2,
    BODY_CS_LENS = 3,
    BODY_POLL_LENS = 4,
    LENS_DETECT = 5,
    LOGIC_VCC_SW = 6,
    LENS_PWR_SW = 7
}; 


//Forward declaring CameraFirmware class so the compiler doesn't throw an error.
//A case of circular dependency.
class CameraFirmware;
class IdleState;
class LinkEstablishmentState;
class SpeedNegotiationState;
class InitializationState;
class RegularMessagingState;
class ShutDownState;
class OffState;

class LensState
{
 public:
  static IdleState idle;
  static LinkEstablishmentState linkEstablishment;
  static SpeedNegotiationState speedNegotiation;
  static InitializationState initialization;
  static RegularMessagingState regularMessaging;
  static ShutDownState shutDown;
  static OffState off;
  
  
  virtual ~LensState(){}
  virtual void enter(CameraFirmware& firmware){}
  virtual void handleInput(CameraFirmware& firmware, EVENT e){}
  virtual void update(CameraFirmware& firmware){}
};
