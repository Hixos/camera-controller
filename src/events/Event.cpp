#include "Event.h"

using std::make_shared;

const EventPtr C_EV_ENTRY = make_shared<const EventSMEntry>();
const EventPtr C_EV_EXIT = make_shared<const EventSMExit>();
const EventPtr C_EV_EMPTY = make_shared<const EventSMEmpty>();
const EventPtr C_EV_INIT = make_shared<const EventSMInit>();