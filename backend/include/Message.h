#pragma once
#include "Arduino.h"
#include "Constants.h"
#include <cstdint>

class Message
{
public:
  Message() = default;
  Message(const byte* messageBuffer);
  
  Message(byte messageClass, byte sequenceNumber, byte messageType, const byte* body, uint16_t messageLength);

  void prepForSending();
  byte getMessageType();
  byte getMessageLength();
  
  
  byte mMessageBuffer[MAX_BUFFER_SIZE] = {0};
  
private:
  void setHeader();
  void setFooter();

protected:
  uint16_t mMessageLength; //Legth of the message from Start Byte to the End Byte(contains checksum and everything between 0xF0 and 0x55)
  byte mMessageClass; 
  byte mSequenceNumber; //Count sequence number for normal and set to 0 for initialization
  byte mMessageType;
  
};
