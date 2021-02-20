#ifndef _BUTTON_HANDLER
#define _BUTTON_HANDLER

#include <Arduino.h>
#include "../../utils/log.h"

struct ButtonUpdate {
  uint8_t index;
  bool down;
  bool downChanged;
  unsigned long prevDuration;
  uint8_t repeat;
  uint8_t longpress;
};

typedef std::function<void (ButtonUpdate update)> ChangeCallback;

struct ButtonAttributes {
  uint8_t index;
  uint8_t pin;
  bool invert;
  bool pullupEnable;
};

struct ButtonState {
  ButtonAttributes attributes;
  bool down;
  unsigned long changeTime;
  uint8_t repeat;
  uint8_t longpress;
  unsigned long longpressTime;
};

typedef std::function<void (ButtonState *button)> EachButtonCallback;

struct ButtonsClassConfig {
  unsigned long debounceTime;
  unsigned long repeatTime;
  unsigned long longpressTime;
  std::vector<ButtonAttributes> buttons;
};

struct ButtonsClassState {
  bool running = false;
  unsigned long debounceTime;
  unsigned long repeatTime;
  unsigned long longpressTime;
  std::vector<ButtonState> buttons;
  ChangeCallback changeCallback = [](ButtonUpdate update) {};
  Log::Callback debugCallback = [](String key, String value) {};
};

class Buttons {
  private:
    ButtonsClassState state;
    void eachButton(EachButtonCallback callback);

  public:
    Buttons(ButtonsClassConfig config);
    void setChangeCallback(ChangeCallback callback);
    void setDebug(Log::Callback callback);
    void start();
    void stop();
    void update();
};

#endif
