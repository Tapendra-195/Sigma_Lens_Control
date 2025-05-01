#pragma once

#include "Arduino.h"
#include "Constants.h"
#include <cstdint>

class Message
{
public:
  Message(const byte* messageBuffer);
  
  Message(byte messageClass, byte sequenceNumber, byte messageType, const byte* body, uint16_t messageLength);

  void prepForSending();
  
  uint16_t messageLength; //Legth of the message from Start Byte to the End Byte(contains checksum and everything between 0xF0 and 0x55)
  byte messageClass; 
  byte sequenceNumber; //Count sequence number for normal and set to 0 for initialization
  byte messageType;
  byte messageBuffer[MAX_BUFFER_SIZE] = {0}; //Contains message from 0xF0 to 0x55 but doesn't contain the(256 bytes long??)
  
private:
  //uint16_t bodyLength; //length of the body of message(only body)
  void setHeader();
  void setFooter();
};
