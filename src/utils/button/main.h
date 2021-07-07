#pragma once

#ifdef IOT_NODE_BUTTONS

#include <Arduino.h>
#include <functional>

#include "../../utils/log.h"

namespace IotNode {
namespace Utils {

namespace Button {
  struct Update {
    bool down;
    bool downChanged;
    unsigned long prevDuration;
    uint8_t repeat;
    uint8_t longpress;
    std::vector<bool> pressedMap;
  };

  typedef std::function<void (Update update)> ChangeCallback;

  struct Config {
    uint8_t pin;
    bool invert;
    bool pullupEnable;
    unsigned long debounceTime;
    unsigned long repeatTime;
    unsigned long longpressTime;
    unsigned long noiseGateTime;
  };

  struct State {
    bool running = false;
    bool down;
    unsigned long changeTime;
    uint8_t repeat;
    uint8_t longpress;
    unsigned long longpressTime;
    unsigned long noiseGateTime;
    ChangeCallback changeCallback = [](Update update) {};
  };

  class Class {
    private:
      Config config;
      State state;

    public:
      Class(Config _config);
      void setChangeCallback(ChangeCallback callback);
      bool isDown();
      void start();
      void stop();
      void update();
      void update(bool insert);
  };
}

} // section namespace
} // project namespace

#endif
