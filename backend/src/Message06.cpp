#include "../include/Message06.h"

Message06::Message06(byte messageClass, byte sequenceNumber, byte messageType, const byte *body, uint16_t messageLength): Message(messageClass, sequenceNumber, messageType, body, messageLength) 
{
}

Message06::Message06(const byte* messageBuffer): Message(messageBuffer) 
{
}

int16_t Message06::getLensPos() 
{
  int16_t lensPos = static_cast<int16_t>((mMessageBuffer[INDEX_FOCUS_H1] << 8) | mMessageBuffer[INDEX_FOCUS_L1]);

  return lensPos;
}
