#pragma once

#ifdef IOT_NODE_BUTTONS

#include <Arduino.h>

#include "../i2c.h"
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
  #elif defined(IOT_NODE_BOARD_ESP_NOW_TEST_BUTTON)
    static const Config button0Config = {
      .pin = 4,
      .invert = false,
      .pullupEnable = true,
      .debounceTime = 50,
      .repeatTime = 3000,
      .longpressTime = 125, // (step duration)
      .noiseGateTime = 0,
    };
    static const Config button1Config = {
      .pin = 14,
      .invert = false,
      .pullupEnable = true,
      .debounceTime = 50,
      .repeatTime = 3000,
      .longpressTime = 125, // (step duration)
      .noiseGateTime = 0,
    };
  #elif defined(IOT_NODE_BOARD_SONOFF_BASIC_R2V13) || defined(IOT_NODE_BOARD_SONOFF_BASIC_R2V1)
    static const Config button0Config = {
      .pin = 0,
      .invert = false,
      .pullupEnable = true,
      .debounceTime = 50,
      .repeatTime = 3000,
      .longpressTime = 125, // (step duration)
      .noiseGateTime = 0,
    };
  #elif defined(IOT_NODE_BOARD_ESP_NOW_TEST_BUTTON_II)
    static const Config button0Config = {
      .pin = 0xff, // insert value on update
      .invert = true,
      .pullupEnable = false,
      .debounceTime = 50,
      .repeatTime = 3000,
      .longpressTime = 125, // (step duration)
      .noiseGateTime = 0,
    };
    static const Config button1Config = {
      .pin = 0xff, // insert value on update
      .invert = true,
      .pullupEnable = false,
      .debounceTime = 50,
      .repeatTime = 3000,
      .longpressTime = 125, // (step duration)
      .noiseGateTime = 0,
    };
  #endif

  extern Class button0;

  #if defined(IOT_NODE_BOARD_SHELLYI3) || defined(IOT_NODE_BOARD_ESP_NOW_TEST_BUTTON) || defined(IOT_NODE_BOARD_ESP_NOW_TEST_BUTTON_II)
    extern Class button1;
  #endif

  #ifdef IOT_NODE_BOARD_SHELLYI3
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
