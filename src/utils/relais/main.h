#ifndef _UTILS_RELAIS_MAIN
#define _UTILS_RELAIS_MAIN

#ifdef IOT_NODE_RELAIS

#include <Arduino.h>
#include "../log.h"

namespace IotNode {
namespace Utils {

namespace Relais {
  struct RegularConfig {
    uint8_t pin;
    bool invert;
  };

  struct PulseConfig {
    uint8_t onPin;
    uint8_t offPin;
    bool invert;
  };

  struct State {
    bool on = false;
    bool wasInitialized = false;
  };

  class Regular {
    private:
      RegularConfig config;
      State state;
      void commit();

    public:
      Regular(RegularConfig _config);
      bool isOn();
      void init();
      void setOn(bool on);
      void toggle();
  };

  class Pulse {
    private:
      PulseConfig config;
      State state;
      void commit();

    public:
      Pulse(PulseConfig _config);
      bool isOn();
      void init();
      void setOn(bool on);
      void toggle();
  };
}

} // section namespace
} // project namespace

#endif

#endif
