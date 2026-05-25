#pragma once

#ifdef IOT_NODE_INDICATORS

#include <Arduino.h>
#include <vector>

#include <pre.h>
#include <post.h>

#include "../log.h"

#ifdef IOT_NODE_SX1509
#include "../sx1509/setup.h"
#endif

#define INDICATOR_BLINK_PERIOD_ON 64
#define INDICATOR_BLINK_PERIOD_OFF 128

namespace IotNode::Utils::Indicator
{
  struct Config
  {
    unsigned char pin;
    bool invert;
    unsigned long blinkPeriodOn;
    unsigned long blinkPeriodOff;
  };

#pragma pack(push, 1)
  struct SequenceItem
  {
    unsigned char value;
    unsigned long time;
    unsigned long rampTime;
  };
#pragma pack(pop)

  typedef ::std::vector<SequenceItem> Sequence;

  struct State
  {
    bool on = false;
    bool wasInitialized = false;
    unsigned long blinkChange = 0;
    unsigned char blinkCount = 0;
    bool blinkInfinite = false;
  };

  class ClassPin
  {
  private:
    Config config;
    State state;
    void commit();

  public:
    ClassPin(Config _config);
    bool isOn();
    void blink(unsigned char count);
    void blink(void);
    void init();
    void setBlinkFrequency(unsigned long blinkPeriodOn, unsigned long blinkPeriodOff);
    void setOn(bool on);
    void toggle();
    void update();
  };

  class ClassExpander
  {
  private:
    Config config;
    State state;
    void commit();

  public:
    ClassExpander(Config _config);
    bool isOn();
    void blink(unsigned char count);
    void blink(void);
    void init();
    void setBlinkFrequency(unsigned long blinkPeriodOn, unsigned long blinkPeriodOff);
    void setOn(bool on);
    void toggle();
    void update();
  };
}

#endif
