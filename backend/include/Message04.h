#pragma once

#include "Message.h"

class Message04 : public Message {
public:
  Message04();
  void setLensPos(int16_t currentLensPos, int16_t targetLensPos);
  void update();
  
private:
  void initialize();
  int16_t mDelLensPos; //2 byte, 2's complement number 
  bool mMoveLens;
  enum BYTE { INDEX_FOCUSING_MODE=9, INDEX_DEL_LENS_POS_L=20, INDEX_DEL_LENS_POS_H=21, MF=0x81, AF=0x83};
};
