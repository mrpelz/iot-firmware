#ifndef _BUTTON_TIMING
#define _BUTTON_TIMING

#include <Arduino.h>
#include <JC_Button_ESP.h>

struct ButtonAttributes {
  uint8_t index;
  uint8_t pin;
  bool invert;
  bool pullupEnable;
};

struct ButtonState {
  uint8_t index;
  Button instance;
  bool isPressed;
  uint32_t lastChange;
};

struct ButtonTimingConfig {
  std::vector<ButtonAttributes> buttons;
  uint32_t debounceTime;
};

struct ButtonTimingState {
  bool begun = false;
  std::vector<ButtonState> buttons;
  std::function<void (
    uint8_t index,
    bool isPressed,
    uint32_t prevDuration
  )> changeCallback = [](uint8_t index, bool isPressed, uint32_t prevDuration) {};
  std::function<void (const String &, const String &)> debugCallback =
    [](String key, String value) {};
};

class ButtonTiming {
  private:
    ButtonTimingState state;

  public:
    ButtonTiming(ButtonTimingConfig config);
    void begin();
    void setChangeCallback(
      std::function<void (
        uint8_t index,
        bool isPressed,
        uint32_t prevDuration
      )> callback);
    void setDebug(std::function<void (const String &, const String &)> callback);
    void update();
};

#endif
