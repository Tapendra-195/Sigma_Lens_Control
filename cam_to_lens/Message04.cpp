#include "Arduino.h"
#include "Message04.h"

Message04::Message04(byte messageClass, byte sequenceNumber, byte messageType, const byte* body, uint16_t messageLength): Message(messageClass, sequenceNumber, messageType, body, messageLength) {
  deltaFocus = 0x00;
  changeFocus = false;
}

void Message04::moveFocus(uint16_t delFocus){
  deltaFocus = delFocus;
  changeFocus = true;
}

void Message04::update(){
  //increment sequence number
  sequenceNumber++;

  if(changeFocus){
    messageLength = 0x001B;
    messageBuffer[19] = 0x1D;
    messageBuffer[22] = 0x00;
    messageBuffer[23] = 0x2C;

    messageBuffer[INDEX_DELTA_FOCUS_L] = deltaFocus & 0xFF;
    messageBuffer[INDEX_DELTA_FOCUS_H] = deltaFocus >> 8;

    changeFocus = false;
  }
  else{
    messageLength = 0x0016;
  }
  
}

