#include "Arduino.h"
#include "Message06.h"

Message06::Message06(byte messageClass, byte sequenceNumber, byte messageType, const byte *body, uint16_t messageLength): Message(messageClass, sequenceNumber, messageType, body, messageLength) {
  getFocus();
}

Message06::Message06(const byte* messageBuffer): Message(messageBuffer) {
  getFocus();
}

uint16_t Message06::getFocus() {
  focus = (static_cast<uint16_t>(messageBuffer[INDEX_FOCUS_H1]) << 8) | static_cast<uint16_t>(messageBuffer[INDEX_FOCUS_L1]);

  return focus;
}
