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
  void event(::std::vector<unsigned char> payload);

  void onDataReceived(const unsigned char *mac, const unsigned char *incomingData, int length);
}

#endif
