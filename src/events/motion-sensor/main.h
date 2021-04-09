#ifndef _EVENTS_MOTION_SENSOR_MAIN
#define _EVENTS_MOTION_SENSOR_MAIN

#ifdef IOT_NODE_MOTION_SENSOR

#include <Arduino.h>
#include <functional>

#include "../../utils/log.h"
#include "../../utils/udp/main.h"
#include "../event-ids.h"

namespace IotNode {
namespace Events {

namespace MotionSensor {
  typedef std::function<void (bool down)> ChangeCallback;

  struct State {
    bool running = false;
    bool down;
    ChangeCallback changeCallback = [](bool down) {};
  };

  class Class {
    private:
      uint8_t pin;
      State state;

    public:
      Class(uint8_t _pin);
      void setChangeCallback(ChangeCallback callback);
      void start();
      void update();
  };

  ChangeCallback makeEvent(Utils::UDP::Class *udp, uint8_t index);
}

} // section namespace
} // project namespace

#endif

#endif
