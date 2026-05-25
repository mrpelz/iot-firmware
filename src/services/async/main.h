#pragma once

#ifdef IOT_NODE_ESP32

#include <Arduino.h>

#include <pre.h>
#include <post.h>

#include "../../utils/log.h"
#include "../../utils/udp/main.h"

#define ASYNC_RESPONSE_DELAY 30000

namespace IotNode::Services::Async
{
  void initializer();

  void responseTask(void *parameter);

  void handler(Utils::UDP::Payload *request, Utils::UDP::RespondCallback respond, Utils::UDP::Peer peer);
}

#endif
