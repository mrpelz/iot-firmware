#pragma once

#ifdef IOT_NODE_BOARD_H801

#include <Arduino.h>

#include <FadeLed.h>

#include "../../services/led/main.h"
#include "../../utils/udp/setup.h"

namespace IotNode::Setup::Led
{
  extern FadeLed led0;
  extern FadeLed led1;
  extern FadeLed led2;
  extern FadeLed led3;
  extern FadeLed led4;

  void update();

  void setup();
}

#endif
