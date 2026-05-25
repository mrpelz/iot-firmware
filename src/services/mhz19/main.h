#pragma once

#ifdef IOT_NODE_MHZ19

#include <Arduino.h>

#include <pre.h>
#include <post.h>

#include "MHZ19.h"

#include "../../utils/log.h"
#include "../../utils/udp/main.h"

namespace IotNode::Services::Mhz19
{
  void initializer();

  void responseTask(void *parameter);

  void handler(Utils::UDP::Payload *request, Utils::UDP::RespondCallback respond, Utils::UDP::Peer peer);
}

#endif
