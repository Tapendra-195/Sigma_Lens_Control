#pragma once
#include "LensState.h"

class IdleState : public LensState
{
 public:
  IdleState() = default;
  void enter(CameraFirmware& firmware) override;
  void handleInput(CameraFirmware& firmware, EVENT e) override;
};










