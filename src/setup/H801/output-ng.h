#pragma once

#ifdef IOT_NODE_BOARD_H801

#include <Arduino.h>

#include "../../services/output-ng/main.h"
#include "../../utils/output-ng/main.h"
#include "../../utils/udp/setup.h"

namespace IotNode::Setup::OutputNg
{
  extern ::IotNode::Utils::OutputNg::Dimmable indicatorR;
  extern ::IotNode::Utils::OutputNg::Dimmable indicatorG;

  extern ::IotNode::Utils::OutputNg::Dimmable ledR;
  extern ::IotNode::Utils::OutputNg::Dimmable ledB;
  extern ::IotNode::Utils::OutputNg::Dimmable ledG;
  extern ::IotNode::Utils::OutputNg::Dimmable ledW1;
  extern ::IotNode::Utils::OutputNg::Dimmable ledW2;

  void update();
  void setup();
}

#endif
