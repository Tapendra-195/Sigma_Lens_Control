#include "../include/Message03.h"

Message03::Message03():Message()
{
  initialize();
}

void Message03::initialize()
{
  mAperture = 0x1400;
  
  //Setting Header
  mMessageLength = 0x20;
  mMessageClass = 0x01;
  mSequenceNumber = 0x10;
  mMessageType = 0x03;

  //Setting Body
  mMessageBuffer[6] = 0x40;
  mMessageBuffer[7] = 0x41;
  mMessageBuffer[8] = 0x00;
  mMessageBuffer[INDEX_APERTURE_L1] = mAperture & 0xFF;
  mMessageBuffer[INDEX_APERTURE_H1] = mAperture >> 8;
  mMessageBuffer[INDEX_APERTURE_L2] = mAperture & 0xFF;
  mMessageBuffer[INDEX_APERTURE_H2] = mAperture >> 8;
  mMessageBuffer[INDEX_OVERWRITTEN] = 0x1C;
  mMessageBuffer[14] = 0x00;
  mMessageBuffer[15] = 0x00;
  mMessageBuffer[16] = ((mAperture>>8) < 0x16)?0x02:0x04;
  mMessageBuffer[INDEX_NOT_ENABLE] = 0x00; 
  mMessageBuffer[18] = 0x00;
  mMessageBuffer[19] = 0x02;
  mMessageBuffer[20] = 0x00;
  mMessageBuffer[21] = 0x02;
  mMessageBuffer[22] = 0x01;
  mMessageBuffer[23] = 0x00;
  mMessageBuffer[24] = 0x00;
  mMessageBuffer[25] = 0x00;
  mMessageBuffer[26] = 0x2F;
  mMessageBuffer[27] = 0x15;
  mMessageBuffer[28] = 0x16;
}

void Message03::update()
{
  //increment sequence number
  mSequenceNumber++;
  
  byte aperture_L = mAperture & 0xFF;
  byte aperture_H = mAperture >> 8;

  //Target 1?
  mMessageBuffer[INDEX_APERTURE_L1] = aperture_L;
  mMessageBuffer[INDEX_APERTURE_H1] = aperture_H;

  //Target 2? (IDK what I am talking about, but they have same value)
  mMessageBuffer[INDEX_APERTURE_L2] = aperture_L;
  mMessageBuffer[INDEX_APERTURE_H2] = aperture_H;

  //Normal byte change from msg to msg
  //messageBuffer[8] = 0x08; 
  mMessageBuffer[28] = mMessageBuffer[28]^0x01; //Starts from 16 then changes between 16 and 17
  mMessageBuffer[16] = (aperture_H < 0x16)?0x02:0x04;
  mMessageBuffer[18] = mMessageBuffer[18]^0x01;
  mMessageBuffer[INDEX_NOT_ENABLE] = mMessageBuffer[INDEX_SEQUENCE_NUMBER]%mMessageBuffer[16];
}

void Message03::setAperture(unsigned int value)
{
  mAperture = value;
}
