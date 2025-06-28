#include "../include/Message.h"


//The whole message is given including 0xF0,...,0x55
Message::Message(const byte* messageBuffer) {
  mMessageLength = (static_cast<uint16_t>(messageBuffer[static_cast<size_t>(HEADER::INDEX_MESSAGE_LENGTH_H)]) << 8) | static_cast<uint16_t>(messageBuffer[static_cast<size_t>(HEADER::INDEX_MESSAGE_LENGTH_L)]);
  //bodyLength = messageLength - HEADER_LENGTH - FOOTER_LENGTH;
  mMessageClass = messageBuffer[static_cast<size_t>(HEADER::INDEX_MESSAGE_CLASS)];
  mSequenceNumber = messageBuffer[static_cast<size_t>(HEADER::INDEX_SEQUENCE_NUMBER)];
  mMessageType = messageBuffer[static_cast<size_t>(HEADER::INDEX_MESSAGE_TYPE)];
  
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
    uint16_t bodyLength = mMessageLength - static_cast<uint16_t>(PADDING::HEADER_LENGTH) - static_cast<uint16_t>(PADDING::FOOTER_LENGTH);
    for (size_t i = 0; i < bodyLength; i++) {
      mMessageBuffer[i + static_cast<size_t>(PADDING::HEADER_LENGTH)] = body[i];
    }
}

void Message::prepForSending() {
  setHeader();
  setFooter();
}

//sets message header
void Message::setHeader(){
  mMessageBuffer[static_cast<size_t>(HEADER::INDEX_START)] = static_cast<byte>(BYTE_VALUE::SOM);
  mMessageBuffer[static_cast<size_t>(HEADER::INDEX_MESSAGE_LENGTH_L)] = mMessageLength & 0xFF;
  mMessageBuffer[static_cast<size_t>(HEADER::INDEX_MESSAGE_LENGTH_H)] = mMessageLength >> 8;
  mMessageBuffer[static_cast<size_t>(HEADER::INDEX_MESSAGE_CLASS)] = mMessageClass;
  mMessageBuffer[static_cast<size_t>(HEADER::INDEX_SEQUENCE_NUMBER)] = mSequenceNumber;
  mMessageBuffer[static_cast<size_t>(HEADER::INDEX_MESSAGE_TYPE)] = mMessageType; 
}

void Message::setFooter(){
  uint16_t checksum = 0;
  
  for (size_t i = 1; i < mMessageLength - static_cast<size_t>(PADDING::FOOTER_LENGTH); i++) {
    checksum += mMessageBuffer[i];
  }
  
  //set checksum
  mMessageBuffer[mMessageLength - static_cast<int>(PADDING::FOOTER_LENGTH)] = checksum & 0xFF; //CHECKSUM_LOW
  mMessageBuffer[mMessageLength - static_cast<int>(PADDING::FOOTER_LENGTH) + 1] = checksum >> 8; //CHECKSUM_HIGH

  //set End Byte
  mMessageBuffer[mMessageLength - static_cast<int>(PADDING::FOOTER_LENGTH) + 2] = static_cast<byte>(BYTE_VALUE::EOM);
}

byte Message::getMessageType()
{
  return mMessageType;
}

byte Message::getMessageLength()
{
  return mMessageLength;
}
