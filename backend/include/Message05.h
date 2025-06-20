#pragma once
#include "Message.h"

class Message05 : public Message {
public:
  Message05(const byte* messageBuffer);
  Message05(byte messageClass = MESSAGE_CLASS_NORMAL, byte sequenceNumber = 0x11, byte messageType = MESSAGE_TYPE_05, const byte* body = normal05, uint16_t messageLength = 0x0075);
    
  uint16_t getAperture();
  uint16_t getApertureDialValue();

private:
  enum BYTE { INDEX_APERTURE_L1 = 6, INDEX_APERTURE_H1 = 7, INDEX_APERTURE_L2 = 8, INDEX_APERTURE_H2 = 9, INDEX_APERTURE_DIAL_L = 23, INDEX_APERTURE_DIAL_H = 24};

};
