#pragma once

#ifdef IOT_NODE_INPUT

#include <Arduino.h>
#include <functional>

#include "../../utils/esp-now-node.h"
#include "../../utils/log.h"
#include "../../utils/udp/main.h"
#include "../event-ids.h"

namespace IotNode
{
  namespace Events
  {

    namespace Input
    {
      typedef std::function<void(bool down)> ChangeCallback;

      struct State
      {
        bool running = false;
        bool down = false;
        unsigned long changeTime = 0;
        ChangeCallback changeCallback = [](bool down) {};
      };

      class Class
      {
      private:
        uint8_t pin;
        bool pullup;
        unsigned long debounceTime;
        State state;

      public:
        Class(uint8_t _pin, bool _pullup, unsigned long _debounceTime);
        void setChangeCallback(ChangeCallback callback);
        void start();
        void update();
        void update(bool force);
      };

      ChangeCallback makeEvent(Utils::UDP::Class *udp, uint8_t index);

#ifdef IOT_NODE_ESP_NOW_NODE
      ChangeCallback makeEspNowEvent(uint8_t index);
#endif
    }

  } // section namespace
} // project namespace

#endif
