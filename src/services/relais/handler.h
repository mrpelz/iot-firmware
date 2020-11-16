#ifndef _RELAIS_HANDLER
#define _RELAIS_HANDLER

#define TOSTRING(x) #x
#define STR(x) TOSTRING(x)

#include <Arduino.h>
#include "../../logging.h"

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
