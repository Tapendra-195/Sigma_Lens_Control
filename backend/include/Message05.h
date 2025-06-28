#pragma once
#include "Message.h"

class Message05 : public Message {
public:
  Message05(const byte* messageBuffer);
  Message05(byte messageClass, byte sequenceNumber, byte messageType, const byte* body, uint16_t messageLength);
    
  uint16_t getAperture(); //gets currently set Aperture.
  uint16_t getApertureDialValue(); //Get the value the Aperture Dial is set at. Camera can set the aperture, so set aperture is not same as the dial value.

private:
  enum BYTE { INDEX_APERTURE_L1 = 6, INDEX_APERTURE_H1 = 7, INDEX_APERTURE_L2 = 8, INDEX_APERTURE_H2 = 9, INDEX_APERTURE_DIAL_L = 23, INDEX_APERTURE_DIAL_H = 24};

};
