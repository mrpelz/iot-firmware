#ifndef _BUTTONS_CONFIG
#define _BUTTONS_CONFIG

#include <Arduino.h>

#include "./main.h"

static const ButtonAttributes button0 = {
  .index = 0,
  .pin = 5,
  .invert = false,
  .pullupEnable = true,
};

static const ButtonsClassConfig buttonsConfig = {
  .debounceTime = 50,
  .repeatTime = 3000,
  .longpressTime = 125, // (step duration)
  .buttons = {
    button0
  },
};

#endif
