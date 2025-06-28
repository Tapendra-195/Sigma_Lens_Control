#pragma once
#include "LensState.h"

enum class PHASE
  {
    WAIT_FOR_CS_HIGH,
    WAIT_FOR_RX_HIGH,
    WAIT_FOR_CS_LOW,
    COMPLETE
  };

enum class STATUS
  {
    IN_PROGRESS,
    SUCCESS,
    FALIURE
  };

class LinkEstablishmentState : public LensState
{
private:
  PHASE mPhase;
public:
  unsigned long startTime;
  LinkEstablishmentState() = default;
  void enter(CameraFirmware& firmware) override;
  void handleInput(CameraFirmware& firmware, EVENT e) override;
  void update(CameraFirmware& firmware) override;

  STATUS pollPhase();
};
