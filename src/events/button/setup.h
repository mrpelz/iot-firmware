#ifndef _BUTTONS_SETUP
#define _BUTTONS_SETUP

#include <Arduino.h>

#include "../../services/relais/handler.h"
#include "../../utils/udp/main.h"
#include "./event.h"
#include "./main.h"

namespace IotNode {

namespace Button {
  static const SingleAttributes button0 = {
    .index = 0,
    .pin = 2,
    .invert = true,
    .pullupEnable = true,
  };

  static const Config config = {
    .debounceTime = 50,
    .repeatTime = 3000,
    .longpressTime = 125, // (step duration)
    .buttons = {
      button0
    },
  };

  void task(void * parameter);

  Class *setup(UDP::Class *udp, Relais *relais);
}

} // project namespace

#endif
