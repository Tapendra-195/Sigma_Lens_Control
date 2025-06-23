#include "../include/Message04.h"

Message04::Message04():Message()
{
  initialize();
}

void Message04::initialize()
{
  mDelLensPos = 0x00;
  mMoveLens = false;

  
  //Setting Header
  mMessageLength = 0x16;
  mMessageClass = 0x01;
  mSequenceNumber = 0x10;
  mMessageType = 0x04;

  //setting Message body
  mMessageBuffer[6] = 0x00;
  mMessageBuffer[7] = 0x00;
  mMessageBuffer[8] = 0x19;
  mMessageBuffer[INDEX_FOCUSING_MODE] = MF;
  mMessageBuffer[10] = 0x00;
  mMessageBuffer[11] = 0x00;
  mMessageBuffer[12] = 0x2D;
  mMessageBuffer[13] = 0x00;
  mMessageBuffer[14] = 0x00;
  mMessageBuffer[15] = 0x00;
  mMessageBuffer[16] = 0x08;
  mMessageBuffer[17] = 0x00;
  mMessageBuffer[18] = 0x00;
}

void Message04::reset()
{
  initialize();
}

void Message04::setLensPos(int16_t currentLensPos, int16_t targetLensPos)
{
  mDelLensPos = targetLensPos - currentLensPos; //This is del lens position
  mMoveLens = true;
}

void Message04::update()
{
  //increment sequence number
  mSequenceNumber = (mSequenceNumber+1)%0xF0;

  if(mMoveLens){
    mMessageLength = 0x001B;

    mMessageBuffer[19] = 0x1D;
    mMessageBuffer[INDEX_DEL_LENS_POS_L] = mDelLensPos & 0xFF;
    mMessageBuffer[INDEX_DEL_LENS_POS_H] = mDelLensPos >> 8;
    mMessageBuffer[22] = 0x00;
    mMessageBuffer[23] = 0x2C;

    mMoveLens = false;
  }
  else{
    mMessageLength = 0x0016;
  }
  
}

