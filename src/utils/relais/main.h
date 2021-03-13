#ifndef _UTILS_RELAIS_MAIN
#define _UTILS_RELAIS_MAIN

#include <Arduino.h>
#include "../log.h"

namespace IotNode {
namespace Utils {

namespace Relais {
  struct Config {
    uint8_t pin;
    bool invert;
  };

  struct State {
    bool on = false;
    bool wasInitialized = false;
  };

  class Class {
    private:
      Config config;
      State state;
      void commit();

    public:
      Class(Config _config);
      bool isOn();
      void init();
      void setOn(bool on);
      void toggle();
  };
}

} // section namespace
} // project namespace

#endif
