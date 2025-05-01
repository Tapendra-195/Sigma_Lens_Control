#pragma once

#include "Arduino.h"
#include "Message.h"
#include <array>

class Message03 : public Message {
public:
  Message03(byte messageClass = MESSAGE_CLASS_NORMAL, byte sequenceNumber = 0x11, byte messageType = MESSAGE_TYPE_03, const byte* body = normal03, uint16_t messageLength = 0x0020);

  void incrementAperture();
  void decrementAperture();
  float getCurrentAperture();
  void update();
  void updateBasedOn05(Message* input); 
  
  uint16_t aperture;
private:
  enum BYTES { INDEX_APERTURE_L1=9, INDEX_APERTURE_H1=10, INDEX_APERTURE_L2=11, INDEX_APERTURE_H2=12 };

  int currentApertureIndex;

  struct ApertureMap{
    float humanAperture; //human readable aperture
    uint16_t lensAperture; //aperture the lens understands
  };

  static constexpr std::array<ApertureMap, 22> apertureTable = {{ {16.0f, 0x1800},
								 {14.0f, 0x17AA},
								 {13.0f, 0x1755},
								 {11.0f, 0x1700},
								 {10.0f, 0x16AA},
								 {9.0f, 0x1655},
								 {8.0f, 0x1600},
								 {7.1f, 0x15AA},
								 {6.3f, 0x1555},
								 {5.6f, 0x1500},
								 {5.0f, 0x14AA},
								 {4.5f, 0x1455},
								 {4.0f, 0x1400},
								 {3.5f, 0x13AA},
								 {3.2f, 0x1355},
								 {2.8f, 0x1300},
								 {2.5f, 0x12AA},
								 {2.2f, 0x1255},
								 {2.0f, 0x1200},
								 {1.8f, 0x11AA},
								 {1.6f, 0x1155},
								 {1.4f, 0x1100} }};
};
