#pragma once

#ifdef IOT_NODE_INDICATORS

#include <Arduino.h>

#include <pre.h>
#include <post.h>
#include <vector>
#include "../log.h"

#ifdef IOT_NODE_SX1509
#include "../sx1509/setup.h"
#endif

#ifdef IOT_NODE_WS2812
#include <ESP32_WS2812_Lib.h>
#endif

#define INDICATOR_BLINK_PERIOD_ON 64
#define INDICATOR_BLINK_PERIOD_OFF 128

namespace IotNode
{
  namespace Utils
  {

    namespace Indicator
    {
      struct Config
      {
        uint8_t pin;
        bool invert;
        unsigned long blinkPeriodOn;
        unsigned long blinkPeriodOff;
      };

#pragma pack(push, 1)
      struct SequenceItem
      {
        uint8_t value;
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
        uint8_t blinkCount = 0;
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
        void blink(uint8_t count);
        void blink(void);
        void init();
        void setBlinkFrequency(unsigned long blinkPeriodOn, unsigned long blinkPeriodOff);
        void setOn(bool on);
        void toggle();
        void update();
      };

#ifdef IOT_NODE_SX1509
      class ClassExpander
      {
      private:
        Config config;
        State state;
        void commit();

      public:
        ClassExpander(Config _config);
        bool isOn();
        void blink(uint8_t count);
        void blink(void);
        void init();
        void setBlinkFrequency(unsigned long blinkPeriodOn, unsigned long blinkPeriodOff);
        void setOn(bool on);
        void toggle();
        void update();
      };
#endif

#ifdef IOT_NODE_WS2812
      class ClassWS2812
      {
      private:
        Config config;
        State state;
        ESP32_WS2812 *bus;
        int busIndex;
        uint8_t busOffset;
        void commit();

      public:
        ClassWS2812(Config _config, ESP32_WS2812 *_bus);
        bool isOn();
        void blink(uint8_t count);
        void blink(void);
        void init();
        void setBlinkFrequency(unsigned long blinkPeriodOn, unsigned long blinkPeriodOff);
        void setOn(bool on);
        void toggle();
        void update();
      };
    }
#endif

  } // section namespace
} // project namespace

#endif
