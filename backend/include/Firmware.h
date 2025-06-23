#include "Arduino.h"
#include "../include/Constants.h"
#include "../include/Message.h"
#include "../include/Message03.h"
#include "../include/Message04.h"
#include "../include/Message05.h"
#include "../include/Message06.h"

class Firmware
{
 public:
  Firmware();

  
  void initialize();
  void begin();
  void run();
  void reset();
  
 private:
  void handleFrontEndInput();
  void pollLens(); //Sends polling signal to lens
  void sendMessage(const byte * message, int length); //Sends Message to lens
  void processByte();//int read, byte *buffer, int &position, int direction);
  Message03 mMessage03;
  Message04 mMessage04;

  void transitionToIdle();
  void transitionToLinkEstablishment();
  void transitionToSpeedNegotiation();
  void transitionToInitialization();
  void transitionToRegularMessaging();
  void transitionToPoweringOff();
  void transitionToOff();
  
  bool mMessageFullyRead = false; //indicates if the whole message is read.
  bool mIsPoweredOn = true;
  // Timing parameters for a 60 Hz pulse
  // Total period = 1/60 sec ≈ 16667 microseconds
  // Low pulse duration is 63.4 microseconds (rounded to 64 for simplicity)
  const unsigned long totalPeriod = 16667UL;   // Total period in microseconds
  const unsigned long lowDuration   = 64UL;//64UL;      // Duration of the LOW pulse in microseconds
  const unsigned long highDuration  = totalPeriod - lowDuration; // Remaining HIGH time

  unsigned long lastPulseTime = 0;

  int lensToBodyBufferPosition = INVALID_POSITION;
  int packetLength = INVALID_POSITION;
  byte lensToBodyBuffer[MAX_BUFFER_SIZE] = {0};
  
  //Data to send back
  int16_t currentLensPos =0x00;
  uint16_t currentAperture = 0x00;
  uint16_t lastApertureDialValue = 0x00;

  uint8_t mResetCount = 0;

  enum PINS{PIN_RX = 0, PIN_TX = 1, PIN_LENS_CS_BODY = 2, PIN_BODY_CS_LENS = 3, PIN_BODY_POLL_LENS = 4, PIN_LENS_DETECT = 5, PIN_LOGIC_VCC_SW = 6, PIN_LENS_PWR_SW = 7}; 
  enum STATE{IDLE, LINK_ESTABLISHMENT, SPEED_NEGOTIATION, INITIALIZATION, REGULAR_MESSAGING, POWERING_OFF, OFF};

  STATE mCurrentState = IDLE;
  STATE mNextState = IDLE;
  bool mPollLens = false;
};
