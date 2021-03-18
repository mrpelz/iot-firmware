#ifndef _UTILS_BUTTON_SETUP
#define _UTILS_BUTTON_SETUP

#include <Arduino.h>

#include "./main.h"

namespace IotNode {
namespace Utils {

namespace Button {
  // static const Config button0Config = {
  //   .pin = 2,
  //   .invert = false,
  //   .pullupEnable = true,
  //   .debounceTime = 50,
  //   .repeatTime = 3000,
  //   .longpressTime = 125, // (step duration)
  // };

  // extern Class button0;

  void update();

  #ifdef ARDUINO_ARCH_ESP32
    void task(void *parameter);
  #endif

  void setup();
}

} // section namespace
} // project namespace

#endif
