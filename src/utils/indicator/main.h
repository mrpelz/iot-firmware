#ifndef _INDICATOR_MAIN
#define _INDICATOR_MAIN

#include <Arduino.h>
#include "../../utils/log.h"

#define WT32_ETH01_RXD_LED 5
#define WT32_ETH01_TXD_LED 17

#define INDICATOR_BLINK_PERIOD 125

namespace IotNode {

namespace Indicator {
  struct Config {
    uint8_t pin;
    bool invert;
    unsigned long blinkPeriod;
  };

  struct State {
    uint8_t pin;
    bool invert = false;
    unsigned long blinkPeriod = 0;
    bool on = false;
    bool wasInitialized = false;
    unsigned long blinkChange = 0;
    uint8_t blinkCount = 0;
    bool blinkInfinite = false;
  };

  class Class {
    private:
      State state;
      void commit();

    public:
      Class(Config config);
      bool isOn();
      void blink(void);
      void blink(uint8_t count);
      void init();
      void setOn(bool on);
      void toggle();
      void update();
  };
}

} // project namespace

#endif
