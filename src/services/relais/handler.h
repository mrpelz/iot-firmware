#ifndef _RELAIS_HANDLER
#define _RELAIS_HANDLER

#include <Arduino.h>
#include "../../utils/log.h"

struct RelaisConfig {
  uint8_t pin;
  bool invert;
};

struct RelaisState {
  uint8_t pin;
  bool invert = false;
  bool on = false;
  bool wasInitialized = false;
};

class Relais {
  private:
    RelaisState state;
    void commit();

  public:
    Relais(RelaisConfig config);
    bool isOn();
    void init();
    void setOn(bool on);
    void toggle();
};

#endif
