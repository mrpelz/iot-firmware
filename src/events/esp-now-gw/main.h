#pragma once

#ifdef IOT_NODE_ESP_NOW_GW

#include <Arduino.h>

#include <pre.h>
#include <post.h>
#include <vector>

#include "../../utils/link/main.h"
#include "../../utils/log.h"
#include "../../utils/udp/setup.h"
#include "../event-ids.h"

namespace IotNode::Events::EspNowGw
{
  void event(::std::vector<uint8_t> payload);

  void onDataReceived(const uint8_t *mac, const uint8_t *incomingData, int length);
}

#endif
