#include "Arduino.h"
#include "Message05.h"

Message05::Message05(byte messageClass, byte sequenceNumber, byte messageType, const byte *body, uint16_t messageLength): Message(messageClass, sequenceNumber, messageType, body, messageLength) {
  getAperture();
}

Message05::Message05(const byte* messageBuffer): Message(messageBuffer) {
  getAperture();
}

uint16_t Message05::getAperture() {
  aperture = (static_cast<uint16_t>(messageBuffer[INDEX_APERTURE_H1]) << 8) | static_cast<uint16_t>(messageBuffer[INDEX_APERTURE_L1]);

  return aperture;
}
