#ifndef _RELAIS_MAIN
#define _RELAIS_MAIN

#include <Arduino.h>
#include "../../utils/log.h"

namespace IotNode {

namespace Relais {
  struct Config {
    uint8_t pin;
    bool invert;
  };

  struct State {
    uint8_t pin;
    bool invert = false;
    bool on = false;
    bool wasInitialized = false;
  };

  class Class {
    private:
      State state;
      void commit();

    public:
      Class(Config config);
      bool isOn();
      void init();
      void setOn(bool on);
      void toggle();
  };
}

} // project namespace

#endif
