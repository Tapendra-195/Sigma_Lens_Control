#include "Arduino.h"
#include "Message.h"


//The whole message is given including 0xF0,...,0x55
Message::Message(const byte* messageBuffer) {
  this->messageLength = (static_cast<uint16_t>(messageBuffer[INDEX_MESSAGE_LENGTH_H]) << 8) | static_cast<uint16_t>(messageBuffer[INDEX_MESSAGE_LENGTH_L]);
  //bodyLength = messageLength - HEADER_LENGTH - FOOTER_LENGTH;
  messageClass = messageBuffer[3];
  sequenceNumber = messageBuffer[4];
  messageType = messageBuffer[5];
  
  //copy the body to the message buffer
  for (int i = 0; i < messageLength; i++) {
    this->messageBuffer[i] = messageBuffer[i];
  }
  
}

//Only body is given, so need to add SOM, MessageLen, MessageClass, SeQNo, MessageType, (body), checksum, 0x55
Message::Message(byte messageClass, byte sequenceNumber, byte messageType, const byte* body, uint16_t messageLength) {
    this->messageLength = messageLength;
    
    this->messageClass = messageClass;
    this->sequenceNumber = sequenceNumber;
    this->messageType = messageType;
        
    //copy the body to the message buffer
    uint16_t bodyLength = this->messageLength - HEADER_LENGTH - FOOTER_LENGTH;
    for (int i = 0; i < bodyLength; i++) {
      this->messageBuffer[i + HEADER_LENGTH] = body[i];
    }
}

void Message::prepForSending() {
  setHeader();
  setFooter();
}

//sets message header
void Message::setHeader(){
  messageBuffer[INDEX_START] = START_BYTE;
  messageBuffer[INDEX_MESSAGE_LENGTH_L] = messageLength & 0xFF;
  messageBuffer[INDEX_MESSAGE_LENGTH_H] = messageLength >> 8;
  messageBuffer[INDEX_MESSAGE_CLASS] = messageClass;
  messageBuffer[INDEX_SEQUENCE_NUMBER] = sequenceNumber;
  messageBuffer[INDEX_MESSAGE_TYPE] = messageType; 
}

void Message::setFooter(){
  uint16_t checksum = 0;

  for (int i = 1; i < messageLength - FOOTER_LENGTH; i++) {
    checksum += messageBuffer[i];
  }
  
  //set checksum
  messageBuffer[messageLength - FOOTER_LENGTH] = checksum & 0xFF; //CHECKSUM_LOW
  messageBuffer[messageLength - FOOTER_LENGTH + 1] = checksum >> 8; //CHECKSUM_HIGH

  //set End Byte
  messageBuffer[messageLength - FOOTER_LENGTH + 2] = END_BYTE;
}
