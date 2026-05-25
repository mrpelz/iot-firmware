#pragma once

#ifdef IOT_NODE_LED

#include <Arduino.h>

#include <FadeLed.h>

namespace IotNode::Utils::Led
{
#ifdef IOT_NODE_BOARD_H801
  extern FadeLed led0;
  extern FadeLed led1;
  extern FadeLed led2;
  extern FadeLed led3;
  extern FadeLed led4;
#endif

  void update();

#ifdef IOT_NODE_ESP32
  void task(void *parameter);
#endif

  void setup();
}

#endif
