#include "Arduino.h"
#include "../include/Message.h"


//The whole message is given including 0xF0,...,0x55
Message::Message(const byte* messageBuffer) {
  mMessageLength = (static_cast<uint16_t>(messageBuffer[INDEX_MESSAGE_LENGTH_H]) << 8) | static_cast<uint16_t>(messageBuffer[INDEX_MESSAGE_LENGTH_L]);
  //bodyLength = messageLength - HEADER_LENGTH - FOOTER_LENGTH;
  mMessageClass = messageBuffer[3];
  mSequenceNumber = messageBuffer[4];
  mMessageType = messageBuffer[5];
  
  //copy the body to the message buffer
  for (int i = 0; i < mMessageLength; i++) {
    mMessageBuffer[i] = messageBuffer[i];
  }
  
}

//Only body is given, so need to add SOM, MessageLen, MessageClass, SeQNo, MessageType, (body), checksum, 0x55
Message::Message(byte messageClass, byte sequenceNumber, byte messageType, const byte* body, uint16_t messageLength) {
    mMessageLength = messageLength;
    
    mMessageClass = messageClass;
    mSequenceNumber = sequenceNumber;
    mMessageType = messageType;
        
    //copy the body to the message buffer
    uint16_t bodyLength = mMessageLength - HEADER_LENGTH - FOOTER_LENGTH;
    for (int i = 0; i < bodyLength; i++) {
      mMessageBuffer[i + HEADER_LENGTH] = body[i];
    }
}

void Message::prepForSending() {
  setHeader();
  setFooter();
}

//sets message header
void Message::setHeader(){
  mMessageBuffer[INDEX_START] = START_BYTE;
  mMessageBuffer[INDEX_MESSAGE_LENGTH_L] = mMessageLength & 0xFF;
  mMessageBuffer[INDEX_MESSAGE_LENGTH_H] = mMessageLength >> 8;
  mMessageBuffer[INDEX_MESSAGE_CLASS] = mMessageClass;
  mMessageBuffer[INDEX_SEQUENCE_NUMBER] = mSequenceNumber;
  mMessageBuffer[INDEX_MESSAGE_TYPE] = mMessageType; 
}

void Message::setFooter(){
  uint16_t checksum = 0;

  for (int i = 1; i < mMessageLength - FOOTER_LENGTH; i++) {
    checksum += mMessageBuffer[i];
  }
  
  //set checksum
  mMessageBuffer[mMessageLength - FOOTER_LENGTH] = checksum & 0xFF; //CHECKSUM_LOW
  mMessageBuffer[mMessageLength - FOOTER_LENGTH + 1] = checksum >> 8; //CHECKSUM_HIGH

  //set End Byte
  mMessageBuffer[mMessageLength - FOOTER_LENGTH + 2] = END_BYTE;
}

byte Message::getMessageType()
{
  return mMessageType;
}

byte Message::getMessageLength()
{
  return mMessageLength;
}
