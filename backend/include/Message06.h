#pragma once
#include "Message.h"

class Message06 : public Message {
public:
  Message06(const byte* messageBuffer);
  Message06(byte messageClass = MESSAGE_CLASS_NORMAL, byte sequenceNumber = 0x11, byte messageType = MESSAGE_TYPE_06, const byte* body = normal06, uint16_t messageLength = 0x0030);
    
  int16_t getLensPos();
    
private:
  enum BYTE { INDEX_FOCUS_L1 = 8, INDEX_FOCUS_H1 = 9, INDEX_FOCUS_L2 = 26, INDEX_FOCUS_H2 = 27 };

};
