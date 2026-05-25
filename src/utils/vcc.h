#pragma once

#ifdef IOT_NODE_VCC

#include <ESP8266WiFi.h>

#define VCC_OVERSAMPLE 10

namespace IotNode::Utils::VCC
{
  void update();

  unsigned short get();
}

#endif
