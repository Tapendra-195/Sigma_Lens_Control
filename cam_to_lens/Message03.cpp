#include "Arduino.h"
#include "Message03.h"

Message03::Message03(byte messageClass, byte sequenceNumber, byte messageType, const byte* body, uint16_t messageLength): Message(messageClass, sequenceNumber, messageType, body, messageLength) {
  currentApertureIndex = 10;//0;
  aperture = apertureTable[currentApertureIndex].lensAperture;
}


void Message03::updateBasedOn05(Message* input) {
  //if (input->bodyLength > 22) {
    //body[17] = input->body[4];
    //body[18] = input->body[5];
  //}
}

void Message03::update(){
  //increment sequence number
  sequenceNumber++;
  
  byte aperture_L = aperture & 0xFF;
  byte aperture_H = aperture >> 8;

  //Target 1?
  messageBuffer[INDEX_APERTURE_L1] = aperture_L;
  messageBuffer[INDEX_APERTURE_H1] = aperture_H;

  //Target 2? (IDK what I am talking about, but they have same value)
  messageBuffer[INDEX_APERTURE_L2] = aperture_L;
  messageBuffer[INDEX_APERTURE_H2] = aperture_H;
}

void Message03::incrementAperture(){
  currentApertureIndex = (currentApertureIndex + 1) % apertureTable.size();
  aperture = apertureTable[currentApertureIndex].lensAperture;
}
void Message03::decrementAperture(){
  currentApertureIndex = (currentApertureIndex - 1) % apertureTable.size();
  aperture = apertureTable[currentApertureIndex].lensAperture;
}

float Message03::getCurrentAperture(){
  return apertureTable[currentApertureIndex].humanAperture;
}
