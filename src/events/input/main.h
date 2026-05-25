#pragma once

#ifdef IOT_NODE_INPUT

#include <Arduino.h>

#include <pre.h>
#include <post.h>
#include <functional>

#include "../../utils/esp-now-node.h"
#include "../../utils/log.h"
#include "../../utils/udp/main.h"
#include "../event-ids.h"

namespace IotNode::Events::Input
{
  typedef ::std::function<void(bool down)> ChangeCallback;

  struct State
  {
    bool running = false;
    bool down = false;
    unsigned long changeTime = 0;
    unsigned long noiseGateTime;
    ChangeCallback changeCallback = [](bool down) {};
  };

  class Class
  {
  private:
    unsigned char pin;
    bool pullup;
    unsigned long debounceTime;
    unsigned long noiseGateTime;
    State state;

  public:
    Class(unsigned char _pin, bool _pullup, unsigned long _debounceTime, unsigned long _noiseGateTime);
    void setChangeCallback(ChangeCallback callback);
    void start();
    void update();
    void update(bool force);
  };

  ChangeCallback makeEvent(Utils::UDP::Class *udp, unsigned char index);

#ifdef IOT_NODE_ESP_NOW_NODE
  ChangeCallback makeEspNowEvent(unsigned char index);
#endif
}

#endif
