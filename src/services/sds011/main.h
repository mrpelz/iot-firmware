#pragma once

#ifdef IOT_NODE_SDS011

#include <Arduino.h>

#include "SdsDustSensor.h"

#include "../../utils/log.h"
#include "../../utils/udp/main.h"

namespace IotNode {
namespace Services {

namespace Sds011 {
  void sleepSensor();

  void initializer();

  void responseTask(void *parameter);

  void handler(Utils::UDP::Payload *request, Utils::UDP::RespondCallback respond, Utils::UDP::Peer peer);
}

} // section namespace
} // project namespace

#endif
