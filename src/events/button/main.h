#ifndef _BUTTON_MAIN
#define _BUTTON_MAIN

#include <Arduino.h>

#include "../../utils/log.h"

namespace IotNode {

namespace Button {
  struct Update {
    uint8_t index;
    bool down;
    bool downChanged;
    unsigned long prevDuration;
    uint8_t repeat;
    uint8_t longpress;
  };

  typedef std::function<void (Update update)> ChangeCallback;

  struct SingleAttributes {
    uint8_t index;
    uint8_t pin;
    bool invert;
    bool pullupEnable;
  };

  struct SingleState {
    SingleAttributes attributes;
    bool down;
    unsigned long changeTime;
    uint8_t repeat;
    uint8_t longpress;
    unsigned long longpressTime;
  };

  typedef std::function<void (SingleState *button)> EachCallback;

  struct Config {
    unsigned long debounceTime;
    unsigned long repeatTime;
    unsigned long longpressTime;
    std::vector<SingleAttributes> buttons;
  };

  struct State {
    bool running = false;
    unsigned long debounceTime;
    unsigned long repeatTime;
    unsigned long longpressTime;
    std::vector<SingleState> buttons;
    ChangeCallback changeCallback = [](Update update) {};
    Log::Callback debugCallback = [](String key, String value) {};
  };

  class Class {
    private:
      State state;
      void eachButton(EachCallback callback);

    public:
      Class(Config config);
      void setChangeCallback(ChangeCallback callback);
      void setDebug(Log::Callback callback);
      void start();
      void stop();
      void update();
  };
}

} // project namespace

#endif
