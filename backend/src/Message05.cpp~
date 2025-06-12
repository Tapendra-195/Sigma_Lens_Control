#include "Arduino.h"
#include "Message05.h"

Message05::Message05(byte messageClass, byte sequenceNumber, byte messageType, const byte *body, uint16_t messageLength): Message(messageClass, sequenceNumber, messageType, body, messageLength) 
{
}

Message05::Message05(const byte* mMessageBuffer): Message(mMessageBuffer) 
{
}

uint16_t Message05::getAperture() 
{
  uint16_t mAperture = (mMessageBuffer[INDEX_APERTURE_H1] << 8) | mMessageBuffer[INDEX_APERTURE_L1];
  return mAperture;
}

uint16_t Message05::getApertureDialValue()
{
  uint16_t mApertureDialValue = (mMessageBuffer[INDEX_APERTURE_DIAL_H] << 8) | mMessageBuffer[INDEX_APERTURE_DIAL_L];
  return mApertureDialValue;
}