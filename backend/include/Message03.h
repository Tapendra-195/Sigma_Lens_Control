#pragma once
#include "Message.h"

class Message03 : public Message {
public:
  Message03();
  void update();
  void setAperture(unsigned int value);
  void reset();
  
private:
  void initialize();
  uint16_t mAperture;
  enum BYTES { INDEX_APERTURE_L1=9, INDEX_APERTURE_H1=10, INDEX_APERTURE_L2=11, INDEX_APERTURE_H2=12, INDEX_OVERWRITTEN=13, INDEX_NOT_ENABLE=17};
};
