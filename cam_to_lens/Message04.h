#pragma once

#include "Arduino.h"
#include "Message.h"
#include <array>

class Message04 : public Message {
public:
  Message04(byte messageClass = MESSAGE_CLASS_NORMAL, byte sequenceNumber = 0x11, byte messageType = MESSAGE_TYPE_04, const byte* body = normal04, uint16_t messageLength = 0x0016);
  void moveFocus(uint16_t delFocus);
  void update();

private:
  uint16_t deltaFocus; //2 byte, 2's complement number 
  bool changeFocus;
  enum BYTE { INDEX_DELTA_FOCUS_L=20, INDEX_DELTA_FOCUS_H=21 };
};
