#pragma once
#include "LensState.h"

class OffState : public LensState
{
public:
  OffState() = default;
  void enter(CameraFirmware& firmware) override;
  void handleInput(CameraFirmware& firmware, EVENT e) override;
};
