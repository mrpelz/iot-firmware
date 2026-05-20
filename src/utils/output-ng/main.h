#pragma once

#ifdef IOT_NODE_INDICATORS

#include <Arduino.h>
#include <cmath>
#include <vector>
#include "../log.h"

#ifdef IOT_NODE_SX1509
#include "../sx1509/setup.h"
#endif

#ifdef IOT_NODE_WS2812
#include <ESP32_WS2812_Lib.h>
#endif

#define OUTPUT_BLINK_PERIOD_ON 500
#define OUTPUT_BLINK_PERIOD_OFF 500
#define OUTPUT_ITERATE_INFINITE 0xffffffff
#define OUTPUT_RAMP_THROTTLE 50
#define OUTPUT_BUZZER_HOLD_TIME 125
#define OUTPUT_BUZZER_PAUSE_TIME 375
#define OUTPUT_BUZZER_DEFAULT_FREQUENCY 3750
#define OUTPUT_BUZZER_C_FREQUENCY 4186
#define OUTPUT_BUZZER_C_SHARP_FREQUENCY 4435
#define OUTPUT_BUZZER_D_FREQUENCY 4699
#define OUTPUT_BUZZER_E_FLAT_FREQUENCY 4978
#define OUTPUT_BUZZER_E_FREQUENCY 5274
#define OUTPUT_BUZZER_F_FREQUENCY 5588
#define OUTPUT_BUZZER_F_SHARP_FREQUENCY 5920
#define OUTPUT_BUZZER_G_FREQUENCY 6272
#define OUTPUT_BUZZER_G_SHARP_FREQUENCY 6645
#define OUTPUT_BUZZER_A_FREQUENCY 7040
#define OUTPUT_BUZZER_B_FLAT_FREQUENCY 7459
#define OUTPUT_BUZZER_B_FREQUENCY 7902
#define OUTPUT_BUZZER_LEDC_RESOLUTION 8
#define OUTPUT_BUZZER_LEDC_CHANNEL 7
#define OUTPUT_DIMMABLE_FREQUENCY 5000
#define OUTPUT_DIMMABLE_RESOLUTION 12
#define OUTPUT_DIMMABLE_GAMMA 2.2
#define OUTPUT_DIMMABLE_RAMP_TIME 30000

constexpr unsigned long outputDimmableFull = (1 << OUTPUT_DIMMABLE_RESOLUTION) - 1;

namespace IotNode
{
  namespace Utils
  {

    namespace OutputNg
    {
      double gamma(double input, double gamma);

      template <typename T>
      struct SequenceItem
      {
        T value;
        unsigned long holdTime;
      };

      template <typename T>
      struct Request
      {
        unsigned long iterations;
        std::vector<SequenceItem<T>> sequence;
      };

      template <typename T>
      struct State
      {
        unsigned long iterations = 0;
        bool isInitialized = false;
        unsigned long nextSequenceStep = 0;
        std::vector<SequenceItem<T>> sequence = {};
        size_t sequencePointer = 0;
      };

      template <typename T>
      class Base
      {
      private:
        std::function<void(T value)> _onCommit;
        std::function<void()> _onInit;
        void _commit();

      protected:
        State<T> _state;
        void _reset();

      public:
        T value;
        T previousValue;
        Base(std::function<void()> onInit, std::function<void(T value)> onCommit, T initialValue);
        bool getIsActive();
        void init();
        void set(T value);
        void setSequence(Request<T> request);
        void update();
      };

      class Ramp
      {
      private:
        unsigned long _throttle;
        unsigned long _duration = 0;
        unsigned long _lastUpdate = 0;
        unsigned long _startTime = 0;
        std::function<void()> _onUpdate = NULL;

      public:
        double progress = 0;
        Ramp(unsigned long throttle);
        double getDelta(double startValue, double endValue);
        unsigned long getDelta(unsigned long startValue, unsigned long endValue);
        void reset();
        void set(unsigned long duration, std::function<void()> onUpdate);
        void update();
      };

      template <typename T>
      struct DimmableValue
      {
        unsigned long rampTime;
        T value;
      };

      class Buzzer : public Base<DimmableValue<unsigned long>>
      {
      private:
        bool _invert;
        bool _isAttached = false;
        char _pin;
        Ramp _ramp;
        void _write(unsigned long value);

      public:
        Buzzer(char pin, bool invert);
        void beep(unsigned long count, unsigned long frequency);
        void beep(unsigned long frequency);
        void beep(void);
        void melody(std::vector<unsigned long> _melody, unsigned long count, unsigned long holdTime, unsigned long pauseTime);
        void melody(std::vector<unsigned long> _melody, unsigned long count);
        void melody(std::vector<unsigned long> _melody);
        void update();
      };

      class Binary : public Base<bool>
      {
      public:
        Binary(char pin, bool invert);
        Binary(std::function<void()> onInit, std::function<void(bool value)> onCommit);
        void blink(unsigned long count);
        void blink(void);
        void setOff();
        void setOn();
      };

      class BinaryPulse : public Binary
      {
      public:
        BinaryPulse(char pinOn, char pinOff, bool invert, char pulseTime);
      };

      class Dimmable : public Base<DimmableValue<double>>
      {
      protected:
        Ramp _ramp;

      public:
        Dimmable(char pin, bool invert);
        Dimmable(std::function<void()> onInit, std::function<void(DimmableValue<double> value)> onCommit);
        void blink(unsigned long count);
        void blink(void);
        void setOff();
        void setOn();
        void update();
      };

      struct DimmableRGBValue
      {
        unsigned long rampTime;
        double r;
        double g;
        double b;
      };

      class DimmableRGB : public Base<DimmableRGBValue>
      {
      protected:
        Ramp _ramp;

      public:
        DimmableRGB(char pinR, char pinG, char pinB, bool invert);
        DimmableRGB(std::function<void()> onInit, std::function<void(DimmableRGBValue value)> onCommit);
        void blink(unsigned long count);
        void blink(void);
        void blinkR(unsigned long count);
        void blinkR(void);
        void blinkG(unsigned long count);
        void blinkG(void);
        void blinkB(unsigned long count);
        void blinkB(void);
        void blinkRGB(unsigned long count);
        void blinkRGB(void);
        void blinkRGBInclusive(unsigned long count);
        void blinkRGBInclusive(void);
        void setOff();
        void setOn();
        void setR();
        void setG();
        void setB();
        void update();
      };

      class DimmableRGBWS2812 : public DimmableRGB
      {
      public:
        DimmableRGBWS2812(char index, ESP32_WS2812 *bus, bool push);
      };
    }

  } // section namespace
} // project namespace

#endif
