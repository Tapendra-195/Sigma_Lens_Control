#pragma once
#include "LensState.h"

class RegularMessagingState : public LensState
{
public:
  RegularMessagingState() = default;
  void enter(CameraFirmware& firmware) override;
  void handleInput(CameraFirmware& firmware, EVENT e) override;
};
