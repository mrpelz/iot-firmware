#pragma once

#include <Arduino.h>

#include <pre.h>
#include <post.h>
#include <functional>

#include "../../utils/log.h"

namespace IotNode
{
  namespace Utils
  {

    namespace Keepalive
    {
      typedef std::function<void()> KeepaliveCallback;

      struct State
      {
        bool running = false;
        bool ticked = false;
        KeepaliveCallback callback;
        unsigned long lastTick = 0;
        unsigned long timeout = 0;
      };

      class Class
      {
      private:
        State _state;

      public:
        Class(unsigned long timeout, KeepaliveCallback callback);
        void start();
        void stop();
        void tick();
        void update();
      };
    }

  } // section namespace
} // project namespace
