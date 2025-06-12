#pragma once

#include "Arduino.h"
#include "Message.h"
#include <array>

class Message03 : public Message {
public:
  Message03(byte messageClass = MESSAGE_CLASS_NORMAL, byte sequenceNumber = 0x11, byte messageType = MESSAGE_TYPE_03, const byte* body = normal03, uint16_t messageLength = 0x0020);
  
  void update();
  void setAperture(unsigned int value);
  void getApertureFromDial();

  
private:
  uint16_t mAperture;
  enum BYTES { INDEX_APERTURE_L1=9, INDEX_APERTURE_H1=10, INDEX_APERTURE_L2=11, INDEX_APERTURE_H2=12 };
};
