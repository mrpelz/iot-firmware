#pragma once

#ifdef IOT_NODE_BOARD_H801

#include <Arduino.h>

#include "../../services/output-ng/main.h"
#include "../../utils/output-ng/main.h"
#include "../../utils/udp/setup.h"

namespace IotNode::Setup::OutputNg
{
  extern ::IotNode::Utils::OutputNg::Dimmable indicator0;
  extern ::IotNode::Utils::OutputNg::Dimmable indicator1;

  extern ::IotNode::Utils::OutputNg::Dimmable led0;
  extern ::IotNode::Utils::OutputNg::Dimmable led1;
  extern ::IotNode::Utils::OutputNg::Dimmable led2;
  extern ::IotNode::Utils::OutputNg::Dimmable led3;
  extern ::IotNode::Utils::OutputNg::Dimmable led4;

  void update();
  void setup();
}

#endif
