#ifndef _UTILS_INDICATOR_MAIN
#define _UTILS_INDICATOR_MAIN

#ifdef IOT_NODE_INDICATORS

#include <Arduino.h>
#include "../../utils/log.h"

#define INDICATOR_BLINK_PERIOD_ON 64
#define INDICATOR_BLINK_PERIOD_OFF 128

namespace IotNode {
namespace Utils {

namespace Indicator {
  struct Config {
    uint8_t pin;
    bool invert;
    unsigned long blinkPeriodOn;
    unsigned long blinkPeriodOff;
  };

  struct State {
    bool on = false;
    bool wasInitialized = false;
    unsigned long blinkChange = 0;
    uint8_t blinkCount = 0;
    bool blinkInfinite = false;
  };

  class Class {
    private:
      Config config;
      State state;
      void commit();

    public:
      Class(Config _config);
      bool isOn();
      void blink(uint8_t count);
      void blink(void);
      void init();
      void setBlinkFrequency(unsigned long blinkPeriodOn, unsigned long blinkPeriodOff);
      void setOn(bool on);
      void toggle();
      void update();
  };
}

} // section namespace
} // project namespace

#endif

#endif
