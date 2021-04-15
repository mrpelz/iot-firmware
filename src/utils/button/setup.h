#ifndef _UTILS_BUTTON_SETUP
#define _UTILS_BUTTON_SETUP

#ifdef IOT_NODE_BUTTONS

#include <Arduino.h>

#include "./main.h"

namespace IotNode {
namespace Utils {

namespace Button {
  #ifdef IOT_NODE_BOARD_SHELLY1
    static const Config button0Config = {
      .pin = 5,
      .invert = true,
      .pullupEnable = false,
      .debounceTime = 50,
      .repeatTime = 3000,
      .longpressTime = 125, // (step duration)
      .noiseGateTime = 0,
    };
  #elif defined(IOT_NODE_BOARD_OBI_JACK)
    static const Config button0Config = {
      .pin = 14,
      .invert = false,
      .pullupEnable = true,
      .debounceTime = 50,
      .repeatTime = 3000,
      .longpressTime = 125, // (step duration)
      .noiseGateTime = 0,
    };
  #elif defined(IOT_NODE_BOARD_SHELLYI3)
    static const Config button0Config = {
      .pin = 14,
      .invert = true,
      .pullupEnable = false,
      .debounceTime = 50,
      .repeatTime = 3000,
      .longpressTime = 125, // (step duration)
      .noiseGateTime = 50,
    };
    static const Config button1Config = {
      .pin = 12,
      .invert = true,
      .pullupEnable = false,
      .debounceTime = 50,
      .repeatTime = 3000,
      .longpressTime = 125, // (step duration)
      .noiseGateTime = 50,
    };
    static const Config button2Config = {
      .pin = 13,
      .invert = true,
      .pullupEnable = false,
      .debounceTime = 50,
      .repeatTime = 3000,
      .longpressTime = 125, // (step duration)
      .noiseGateTime = 50,
    };
  #elif defined(IOT_NODE_BOARD_ESP_NOW_TEST_NODE)
    static const Config button0Config = {
      .pin = 4,
      .invert = false,
      .pullupEnable = true,
      .debounceTime = 50,
      .repeatTime = 3000,
      .longpressTime = 125, // (step duration)
      .noiseGateTime = 0,
    };
  #endif

  extern Class button0;

  #ifdef IOT_NODE_BOARD_SHELLYI3
    extern Class button1;
    extern Class button2;
  #endif

  void update();

  #ifdef IOT_NODE_ESP32
    void task(void *parameter);
  #endif

  void setup();
}

} // section namespace
} // project namespace

#endif

#endif
