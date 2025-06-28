#pragma once
#include "Arduino.h"
#include "Constants.h"
#include <cstdint>

enum class MESSAGE_CLASS
  {
    INIT = 0x02,  //During Initialization
    NORMAL = 0x01 //after initialization
  };
enum class HEADER
  {
    INDEX_START=0,            //start byte at index 0
    INDEX_MESSAGE_LENGTH_L=1, //Low byte of message length at index 1
    INDEX_MESSAGE_LENGTH_H=2, //High byte of message length at index 2
    INDEX_MESSAGE_CLASS=3,    //Message class at index 3
    INDEX_SEQUENCE_NUMBER=4,  //Sequence number at index 4
    INDEX_MESSAGE_TYPE=5      //Message Type at index 5
  };

enum class PADDING
  {
    HEADER_LENGTH=6, // START_BYTE, MESSAGE_LENGTH_LOW, MESSAGE_LENGTH_HIGH, MESSAGE_CLASS, SEQUENCE_NUMBER, MESSAGE_TYPE
    FOOTER_LENGTH=3  // CHECKSUM_LOW, CHECKSUM_HIGH, END_BYTE
  };
enum class BYTE_VALUE
  {
    SOM = 0xF0, //Start of the Message
    EOM = 0x55    //End of the Message
  };


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
