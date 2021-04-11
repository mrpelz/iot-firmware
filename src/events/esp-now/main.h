#ifndef _EVENTS_ESP_NOW_MAIN
#define _EVENTS_ESP_NOW_MAIN

#ifdef IOT_NODE_ESP_NOW_GW

#include <Arduino.h>

#include "../../utils/link/main.h"
#include "../../utils/log.h"
#include "../../utils/udp/setup.h"
#include "../event-ids.h"

namespace IotNode {
namespace Events {

namespace EspNow {
  void event(std::vector<uint8_t> payload);

  void onDataReceived(const uint8_t *mac, const uint8_t *incomingData, int length);
}

} // section namespace
} // project namespace

#endif

#endif
