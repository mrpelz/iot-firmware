#ifndef _UTILS_LED_MAIN
#define _UTILS_LED_MAIN

#ifdef IOT_NODE_LED

#include <Arduino.h>
#include "../log.h"

#define ESP8266_PWM_MIN 0
#define ESP8266_PWM_MAX 1023

namespace IotNode {
namespace Utils {

namespace Led {
  struct Config {
    uint8_t pin;
    bool invert;
  };

  struct State {
    uint16_t duty = 0;
    bool wasInitialized = false;
  };

  class Class {
    private:
      Config config;
      State state;
      void commit();

    public:
      Class(Config _config);
      void init();
      bool isOn();
      void setDutyCycle(uint16_t duty);
      void setOn(bool on);
      void toggle();
  };
}

} // section namespace
} // project namespace

#endif

#endif
