#include "../include/LensState.h"
#include "../include/IdleState.h"
#include "../include/LinkEstablishmentState.h"
#include "../include/SpeedNegotiationState.h"
#include "../include/InitializationState.h"
#include "../include/RegularMessagingState.h"
#include "../include/ShutDownState.h"
#include "../include/OffState.h"


IdleState LensState::idle;
LinkEstablishmentState LensState::linkEstablishment;
SpeedNegotiationState LensState::speedNegotiation;
InitializationState LensState::initialization;
RegularMessagingState LensState::regularMessaging;
ShutDownState LensState::shutDown;
OffState LensState::off;
