#include "Arduino.h"
#include "Constants.h"
#include "Message.h"
#include "Message03.h"
#include "Message04.h"
#include "Message05.h"
#include "Message06.h"
#include "LensState.h"
#include "IdleState.h"
#include "LensState.h"
#include "LinkEstablishmentState.h"
#include "SpeedNegotiationState.h"
#include "InitializationState.h"
#include "RegularMessagingState.h"
#include "ShutDownState.h"
#include "OffState.h"
#include "RingBuffer.h"

#include "HumiditySensor.h"

class CameraFirmware
{
 public:
  CameraFirmware();
  void run();
  
  Message03 mMessage03;
  Message04 mMessage04;
  LensState* mState;
  uint8_t mResetCount = 0;
  bool mPollLens = false; //Indicates if the firmware should send polling signal to the lens
  void sendMessage(const byte * message, int length); //Sends Message to lens
  byte lensToBodyBuffer[MAX_BUFFER_SIZE] = {0};
  
  //Data to send back
  struct LensStatus
  {
  public:
    int16_t currentLensPos =0x00;
    uint16_t currentAperture = 0x00;
    uint16_t apertureDialValue = 0x00;
    String currentState  = "";
    String extra = ""; //Can attach humidity value to extra
    
  } lensStatus;

  String getStatus();

  void attachHumiditySensor(HumiditySensor* h)
  {
    humiditySensor = h;
  }
  
 private:
  static void lensDetectISR();
  void handleFrontEndInput();
  void pollLens(); //Sends polling signal to lens
  void processByte();//int read, byte *buffer, int &position, int direction);
  void updateFSM();
    //void sendMessage03();
    //void sendMessage04();

  static RingBuffer<EVENT, 20> mInputBuffer;
    
  // Timing parameters for a 60 Hz pulse
  // Total period = 1/60 sec ≈ 16667 microseconds
  // Low pulse duration is 63.4 microseconds (rounded to 64 for simplicity)
  const unsigned long totalPeriod = 16667UL;   // Total period in microseconds
  const unsigned long lowDuration   = 64UL;//64UL;      // Duration of the LOW pulse in microseconds
  const unsigned long highDuration  = totalPeriod - lowDuration; // Remaining HIGH time

  unsigned long lastPulseTime = 0;

  //for reading the message from lens into the buffer
  const int INVALID_POSITION = -999;
  int lensToBodyBufferPosition = INVALID_POSITION;
  int packetLength = INVALID_POSITION;

  HumiditySensor* humiditySensor = nullptr;
};


