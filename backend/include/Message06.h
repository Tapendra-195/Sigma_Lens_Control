#pragma once
#include "Message.h"

class Message06 : public Message
{
public:
    Message06() = default;
    Message06(const byte* messageBuffer);
    Message06(byte messageClass, byte sequenceNumber, byte messageType, const byte* body, uint16_t messageLength);
    
    int16_t getLensPos();
    
private:
    enum BYTE { INDEX_FOCUS_L1 = 8, INDEX_FOCUS_H1 = 9, INDEX_FOCUS_L2 = 26, INDEX_FOCUS_H2 = 27 };

};
