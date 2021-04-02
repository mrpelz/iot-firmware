#ifndef _UTILS_BUTTON_MAIN
#define _UTILS_BUTTON_MAIN

#ifdef IOT_NODE_BUTTONS

#include <Arduino.h>
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
  };

  typedef std::function<void (Update update)> ChangeCallback;

  struct Config {
    uint8_t pin;
    bool invert;
    bool pullupEnable;
    unsigned long debounceTime;
    unsigned long repeatTime;
    unsigned long longpressTime;
  };

  struct State {
    bool running = false;
    bool down;
    unsigned long changeTime;
    uint8_t repeat;
    uint8_t longpress;
    unsigned long longpressTime;
    ChangeCallback changeCallback = [](Update update) {};
    Utils::Log::Callback debugCallback = [](String key, String value) {};
  };

  class Class {
    private:
      Config config;
      State state;

    public:
      Class(Config _config);
      void setChangeCallback(ChangeCallback callback);
      void setDebug(Utils::Log::Callback callback);
      void start();
      void stop();
      void update();
  };
}

} // section namespace
} // project namespace

#endif

#endif
