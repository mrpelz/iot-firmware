#pragma once

#ifdef IOT_NODE_OUTPUT

#include <Arduino.h>

#include <pre.h>
#include <post.h>
#include "../log.h"

namespace IotNode::Utils::Output
{
  struct RegularConfig
  {
    unsigned char pin;
    bool invert;
  };

  struct PulseConfig
  {
    unsigned char onPin;
    unsigned char offPin;
    bool invert;
  };

  struct State
  {
    bool on = false;
    bool wasInitialized = false;
  };

  class Regular
  {
  private:
    RegularConfig config;
    State state;
    void commit();

  public:
    Regular(RegularConfig _config);
    bool isOn();
    void init();
    void setOn(bool on);
    void toggle();
  };

  class Pulse
  {
  private:
    PulseConfig config;
    State state;
    void commit();

  public:
    Pulse(PulseConfig _config);
    bool isOn();
    void init();
    void setOn(bool on);
    void toggle();
  };
}

#endif
