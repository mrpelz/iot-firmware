#ifndef _BUTTONS_SETUP
#define _BUTTONS_SETUP

#include <Arduino.h>

#include "./event.h"
#include "./main.h"

namespace IotNode {

namespace Button {
  static const SingleAttributes button0 = {
    .index = 0,
    .pin = 2,
    .invert = true,
    .pullupEnable = true,
  };

  static const Config config = {
    .debounceTime = 50,
    .repeatTime = 3000,
    .longpressTime = 125, // (step duration)
    .buttons = {
      button0
    },
  };

  void update();

  #ifdef ARDUINO_ARCH_ESP32
    void task(void * parameter);
  #endif

  Class *setup();
}

} // project namespace

#endif
