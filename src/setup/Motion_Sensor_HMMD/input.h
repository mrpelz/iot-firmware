#pragma once

#ifdef IOT_NODE_BOARD_MOTION_SENSOR_HMMD

#include <Arduino.h>

#include "../../events/input/main.h"
#include "../../utils/udp/setup.h"

namespace IotNode::Setup::Input
{
  extern ::IotNode::Events::Input::Class input0;
  extern ::IotNode::Events::Input::Class input1;
  extern ::IotNode::Events::Input::Class input2;

  void update();

  void task(void *parameter);

  void setup();
}

#endif
