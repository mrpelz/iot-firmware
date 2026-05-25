#pragma once

#ifdef IOT_NODE_SDS011

#include <Arduino.h>

#include <pre.h>
#include <post.h>

#include "SdsDustSensor.h"

#include "../../utils/log.h"
#include "../../utils/udp/main.h"

namespace IotNode::Services::Sds011
{
  void sleepSensor();

  void initializer();

  void responseTask(void *parameter);

  void handler(Utils::UDP::Payload *request, Utils::UDP::RespondCallback respond, Utils::UDP::Peer peer);
}

#endif
