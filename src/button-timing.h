#ifndef _BUTTON_TIMING
#define _BUTTON_TIMING

#include <Arduino.h>
#include "./logging.h"

typedef std::function<void (
  uint8_t index,
  bool down,
  bool downChanged,
  unsigned long prevDuration,
  uint8_t repeat,
  uint8_t longpress
)> ChangeCallback;

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

struct ButtonTimingConfig {
  unsigned long debounceTime;
  unsigned long repeatTime;
  unsigned long longpressTime;
  std::vector<ButtonAttributes> buttons;
};

struct ButtonTimingState {
  bool running = false;
  unsigned long debounceTime;
  unsigned long repeatTime;
  unsigned long longpressTime;
  std::vector<ButtonState> buttons;
  ChangeCallback changeCallback = [](
    uint8_t index,
    bool down,
    bool downChanged,
    unsigned long prevDuration,
    uint8_t repeat,
    uint8_t longpress
  ) {};
  LoggingCallback debugCallback = [](String key, String value) {};
};

class ButtonTiming {
  private:
    ButtonTimingState state;
    void eachButton(EachButtonCallback callback);

  public:
    ButtonTiming(ButtonTimingConfig config);
    void setChangeCallback(ChangeCallback callback);
    void setDebug(LoggingCallback callback);
    void start();
    void stop();
    void update();
};

#endif
