#include "./main.h"

#ifdef IOT_NODE_INDICATORS

namespace IotNode
{
  namespace Utils
  {

    namespace OutputNg
    {
      double gamma(double input, double gamma = OUTPUT_DIMMABLE_GAMMA)
      {
        return pow(input, gamma);
      }

      template <typename T>
      Base<T>::Base(std::function<void()> onInit, std::function<void(T value)> onCommit, T initialValue)
      {
        _onInit = onInit;
        _onCommit = onCommit;
        value = initialValue;
        previousValue = initialValue;

        reset();
      }

      template <typename T>
      void Base<T>::commit()
      {
        if (!_state.isInitialized)
          return;

        _onCommit(value);
      }

      template <typename T>
      void Base<T>::init()
      {
        _onInit();
        _state.isInitialized = true;
      }

      template <typename T>
      void Base<T>::set(T value_)
      {
        reset();
        previousValue = value;
        value = value_;

        commit();
      }

      template <typename T>
      void Base<T>::setSequence(Request<T> request)
      {
        reset();
        _state.iterations = request.iterations;
        _state.sequence = request.sequence;
      }

      template <typename T>
      void Base<T>::update()
      {
        auto now = millis();
        if (now < _state.nextSequenceStep)
        {
          return;
        }

        auto sequenceSize = _state.sequence.size();

        if (!_state.iterations ||
            !sequenceSize ||
            sequenceSize < _state.sequencePointer + 1)
        {
          reset();
          return;
        }

#ifdef IOT_NODE_LOGGING
        Log::debug("update.sequenceSize", String(_state.sequence.size()));
        Log::debug("update.iterations", String(_state.iterations));
        Log::debug("update.sequencePointer", String(_state.sequencePointer));
#endif

        auto sequenceItem = _state.sequence.at(_state.sequencePointer);

        previousValue = value;
        value = sequenceItem.value;
        _state.nextSequenceStep = now + sequenceItem.holdTime;
        _state.sequencePointer = _state.sequencePointer + 1;

        if (_state.sequencePointer == sequenceSize)
        {
          if (_state.iterations != OUTPUT_ITERATE_INFINITE)
          {
            _state.iterations -= 1;
          }

          if (!_state.iterations)
          {
            reset();
          }

          _state.sequencePointer = 0;
        }

        commit();
      }

      template <typename T>
      void Base<T>::reset()
      {
        _state.iterations = 0;
        _state.nextSequenceStep = 0;
        _state.sequence.clear();
        _state.sequencePointer = 0;
      }

      template class Base<unsigned long>;
      Buzzer::Buzzer(char pin) : Base<unsigned long>([pin]()
                                                     { ledcAttach(pin, 1, 1); },
                                                     [pin](unsigned long value)
                                                     {
                                                       ledcWriteTone(pin, value);
                                                     },
                                                     0) {}

      template class Base<bool>;
      Binary::Binary(
          char pin,
          bool invert) : Base<bool>([pin]()
                                    { pinMode(pin, OUTPUT); },
                                    [pin, invert](bool value)
                                    { digitalWrite(pin, (invert ? !value : value)); },
                                    false)
      {
      }
      Binary::Binary(
          std::function<void()> onInit,
          std::function<void(bool value)> onCommit) : Base<bool>(onInit,
                                                                 onCommit,
                                                                 false)
      {
      }

      void Binary::setOn()
      {
        set(true);
      }

      void Binary::setOff()
      {
        set(false);
      }

      void Binary::blink()
      {
        blink(OUTPUT_ITERATE_INFINITE);
      }

      void Binary::blink(unsigned long count)
      {
        reset();
        setSequence({.iterations = count,
                     .sequence = {{
                                      .value = true,
                                      .holdTime = OUTPUT_BLINK_PERIOD_ON,
                                  },
                                  {
                                      .value = false,
                                      .holdTime = OUTPUT_BLINK_PERIOD_OFF,
                                  }}});
      }

      BinaryPulse::BinaryPulse(
          char pinOn,
          char pinOff,
          bool invert,
          char pulseTime = 50) : Binary([pinOn, pinOff]()
                                        {
                                  pinMode(pinOn, OUTPUT);
                                  pinMode(pinOff, OUTPUT); },
                                        [pinOn, pinOff, invert, pulseTime](bool value)
                                        {
                                          digitalWrite(value ? pinOn : pinOff, !invert);
                                          delay(pulseTime);

                                          digitalWrite(value ? pinOn : pinOff, invert);
                                        }) {}

      Ramp::Ramp(unsigned long throttle = OUTPUT_RAMP_THROTTLE)
      {
        _throttle = throttle;
      }

      double Ramp::getDelta(double startValue, double endValue)
      {
        auto delta = endValue - startValue;
        return startValue + (delta * progress);
      }

      void Ramp::reset()
      {
        _duration = 0;
        _lastUpdate = 0;
        _onUpdate = NULL;
        _startTime = 0;
        progress = 0;
      }

      void Ramp::set(unsigned long duration, std::function<void()> onUpdate)
      {
        _duration = duration;
        _onUpdate = onUpdate;
        _startTime = millis();
      }

      void Ramp::update()
      {
        if (!_startTime || !_duration || !_onUpdate || !_throttle)
          return;

        auto now = millis();

        auto timeSinceLastUpdate = now - _lastUpdate;
        if (timeSinceLastUpdate < _throttle)
          return;

        _lastUpdate = now;
        auto timeSinceStart = now - _startTime;

        progress = (double)timeSinceStart / (double)_duration;
        if (progress >= 1)
        {
          progress = 1;
        }

        _onUpdate();

        if (progress == 1)
        {
          reset();
        }
      }

      template class Base<DimmableValue>;
      Dimmable::Dimmable(char pin, bool invert) : Base<DimmableValue>(
                                                      [pin, invert]()
                                                      {
                                                        ledcAttach(pin, OUTPUT_DIMMABLE_FREQUENCY, OUTPUT_DIMMABLE_RESOLUTION);
                                                        ledcOutputInvert(pin, invert);
                                                      },
                                                      [this, pin, invert](DimmableValue value)
                                                      {
                                                        if (value.rampTime)
                                                        {
                                                          _ramp.set(value.rampTime, [this, pin, value]()
                                                                    { ledcWrite(pin, gamma(_ramp.getDelta(previousValue.value, value.value)) * outputDimmableFull); });

                                                          return;
                                                        }

                                                        ledcWrite(pin, gamma(value.value) * outputDimmableFull);
                                                      },
                                                      {.rampTime = 0,
                                                       .value = 0})
      {
        _ramp = Ramp();
      }
      Dimmable::Dimmable(
          std::function<void()> onInit,
          std::function<void(DimmableValue value)> onCommit) : Base<DimmableValue>(onInit, onCommit,
                                                                                   {.rampTime = 0,
                                                                                    .value = 0})
      {
        _ramp = Ramp();
      }

      void Dimmable::blink()
      {
        blink(OUTPUT_ITERATE_INFINITE);
      }

      void Dimmable::blink(unsigned long count)
      {
        reset();
        setSequence({.iterations = count,
                     .sequence = {{
                                      .value = {
                                          .rampTime = OUTPUT_DIMMABLE_RAMP_TIME,
                                          .value = 1,
                                      },
                                      .holdTime = OUTPUT_BLINK_PERIOD_ON + OUTPUT_DIMMABLE_RAMP_TIME,
                                  },
                                  {
                                      .value = {
                                          .rampTime = OUTPUT_DIMMABLE_RAMP_TIME,
                                          .value = 0,
                                      },
                                      .holdTime = OUTPUT_BLINK_PERIOD_OFF + OUTPUT_DIMMABLE_RAMP_TIME,
                                  }}});
      }

      void Dimmable::setOff()
      {
        set({.rampTime = 0,
             .value = 0});
      }

      void Dimmable::setOn()
      {
        set({.rampTime = 0,
             .value = 1});
      }

      void Dimmable::update()
      {
        _ramp.update();
        Base::update();
      }

      template class Base<DimmableRGBValue>;
      DimmableRGB::DimmableRGB(char pinR, char pinG, char pinB, bool invert) : Base<DimmableRGBValue>(
                                                                                   [pinR, pinG, pinB, invert]()
                                                                                   {
                                                                                     ledcAttach(pinR, OUTPUT_DIMMABLE_FREQUENCY, OUTPUT_DIMMABLE_RESOLUTION);
                                                                                     ledcOutputInvert(pinR, invert);

                                                                                     ledcAttach(pinG, OUTPUT_DIMMABLE_FREQUENCY, OUTPUT_DIMMABLE_RESOLUTION);
                                                                                     ledcOutputInvert(pinG, invert);

                                                                                     ledcAttach(pinB, OUTPUT_DIMMABLE_FREQUENCY, OUTPUT_DIMMABLE_RESOLUTION);
                                                                                     ledcOutputInvert(pinB, invert);
                                                                                   },
                                                                                   [this, pinR, pinG, pinB, invert](DimmableRGBValue value)
                                                                                   {
                                                                                     if (value.rampTime)
                                                                                     {
                                                                                       _ramp.set(value.rampTime, [this, pinR, pinG, pinB, value]()
                                                                                                 {
                                                                      ledcWrite(pinR, gamma(_ramp.getDelta(previousValue.r, value.r)) * outputDimmableFull);
                                                                      ledcWrite(pinG, gamma(_ramp.getDelta(previousValue.g, value.g)) * outputDimmableFull);
                                                                      ledcWrite(pinB, gamma(_ramp.getDelta(previousValue.b, value.b)) * outputDimmableFull); });

                                                                                       return;
                                                                                     }

                                                                                     ledcWrite(pinR, gamma(value.r) * outputDimmableFull);
                                                                                     ledcWrite(pinG, gamma(value.g) * outputDimmableFull);
                                                                                     ledcWrite(pinB, gamma(value.b) * outputDimmableFull);
                                                                                   },
                                                                                   {.rampTime = 0,
                                                                                    .r = 0,
                                                                                    .g = 0,
                                                                                    .b = 0})
      {
        _ramp = Ramp();
      }
      DimmableRGB::DimmableRGB(
          std::function<void()> onInit,
          std::function<void(DimmableRGBValue value)> onCommit) : Base<DimmableRGBValue>(onInit, onCommit,
                                                                                         {.rampTime = 0,
                                                                                          .r = 0,
                                                                                          .g = 0,
                                                                                          .b = 0})
      {
        _ramp = Ramp();
      }

      void DimmableRGB::blink()
      {
        blink(OUTPUT_ITERATE_INFINITE);
      }

      void DimmableRGB::blink(unsigned long count)
      {
        reset();
        setSequence({.iterations = count,
                     .sequence = {{
                                      .value = {
                                          .rampTime = OUTPUT_DIMMABLE_RAMP_TIME,
                                          .r = 1,
                                          .g = 1,
                                          .b = 1,
                                      },
                                      .holdTime = OUTPUT_BLINK_PERIOD_ON + OUTPUT_DIMMABLE_RAMP_TIME,
                                  },
                                  {
                                      .value = {
                                          .rampTime = OUTPUT_DIMMABLE_RAMP_TIME,
                                          .r = 0,
                                          .g = 0,
                                          .b = 0,
                                      },
                                      .holdTime = OUTPUT_BLINK_PERIOD_OFF + OUTPUT_DIMMABLE_RAMP_TIME,
                                  }}});
      }

      void DimmableRGB::blinkR()
      {
        blinkR(OUTPUT_ITERATE_INFINITE);
      }

      void DimmableRGB::blinkR(unsigned long count)
      {
        reset();
        setSequence({.iterations = count,
                     .sequence = {{
                                      .value = {
                                          .rampTime = OUTPUT_DIMMABLE_RAMP_TIME,
                                          .r = 1,
                                          .g = 0,
                                          .b = 0,
                                      },
                                      .holdTime = OUTPUT_BLINK_PERIOD_ON + OUTPUT_DIMMABLE_RAMP_TIME,
                                  },
                                  {
                                      .value = {
                                          .rampTime = OUTPUT_DIMMABLE_RAMP_TIME,
                                          .r = 0,
                                          .g = 0,
                                          .b = 0,
                                      },
                                      .holdTime = OUTPUT_BLINK_PERIOD_OFF + OUTPUT_DIMMABLE_RAMP_TIME,
                                  }}});
      }

      void DimmableRGB::blinkG()
      {
        blinkG(OUTPUT_ITERATE_INFINITE);
      }

      void DimmableRGB::blinkG(unsigned long count)
      {
        reset();
        setSequence({.iterations = count,
                     .sequence = {{
                                      .value = {
                                          .rampTime = OUTPUT_DIMMABLE_RAMP_TIME,
                                          .r = 0,
                                          .g = 1,
                                          .b = 0,
                                      },
                                      .holdTime = OUTPUT_BLINK_PERIOD_ON + OUTPUT_DIMMABLE_RAMP_TIME,
                                  },
                                  {
                                      .value = {
                                          .rampTime = OUTPUT_DIMMABLE_RAMP_TIME,
                                          .r = 0,
                                          .g = 0,
                                          .b = 0,
                                      },
                                      .holdTime = OUTPUT_BLINK_PERIOD_OFF + OUTPUT_DIMMABLE_RAMP_TIME,
                                  }}});
      }

      void DimmableRGB::blinkB()
      {
        blinkB(OUTPUT_ITERATE_INFINITE);
      }

      void DimmableRGB::blinkB(unsigned long count)
      {
        reset();
        setSequence({.iterations = count,
                     .sequence = {{
                                      .value = {
                                          .rampTime = OUTPUT_DIMMABLE_RAMP_TIME,
                                          .r = 0,
                                          .g = 0,
                                          .b = 1,
                                      },
                                      .holdTime = OUTPUT_BLINK_PERIOD_ON + OUTPUT_DIMMABLE_RAMP_TIME,
                                  },
                                  {
                                      .value = {
                                          .rampTime = OUTPUT_DIMMABLE_RAMP_TIME,
                                          .r = 0,
                                          .g = 0,
                                          .b = 0,
                                      },
                                      .holdTime = OUTPUT_BLINK_PERIOD_OFF + OUTPUT_DIMMABLE_RAMP_TIME,
                                  }}});
      }

      void DimmableRGB::blinkRGB()
      {
        blinkRGB(OUTPUT_ITERATE_INFINITE);
      }

      void DimmableRGB::blinkRGB(unsigned long count)
      {
        reset();
        setSequence({.iterations = count,
                     .sequence = {{
                                      .value = {
                                          .rampTime = OUTPUT_DIMMABLE_RAMP_TIME,
                                          .r = 1,
                                          .g = 0,
                                          .b = 0,
                                      },
                                      .holdTime = OUTPUT_BLINK_PERIOD_ON + OUTPUT_DIMMABLE_RAMP_TIME,
                                  },
                                  {
                                      .value = {
                                          .rampTime = OUTPUT_DIMMABLE_RAMP_TIME,
                                          .r = 0,
                                          .g = 0,
                                          .b = 0,
                                      },
                                      .holdTime = OUTPUT_BLINK_PERIOD_OFF + OUTPUT_DIMMABLE_RAMP_TIME,
                                  },
                                  {
                                      .value = {
                                          .rampTime = OUTPUT_DIMMABLE_RAMP_TIME,
                                          .r = 0,
                                          .g = 1,
                                          .b = 0,
                                      },
                                      .holdTime = OUTPUT_BLINK_PERIOD_ON + OUTPUT_DIMMABLE_RAMP_TIME,
                                  },
                                  {
                                      .value = {
                                          .rampTime = OUTPUT_DIMMABLE_RAMP_TIME,
                                          .r = 0,
                                          .g = 0,
                                          .b = 0,
                                      },
                                      .holdTime = OUTPUT_BLINK_PERIOD_OFF + OUTPUT_DIMMABLE_RAMP_TIME,
                                  },
                                  {
                                      .value = {
                                          .rampTime = OUTPUT_DIMMABLE_RAMP_TIME,
                                          .r = 0,
                                          .g = 0,
                                          .b = 1,
                                      },
                                      .holdTime = OUTPUT_BLINK_PERIOD_ON + OUTPUT_DIMMABLE_RAMP_TIME,
                                  },
                                  {
                                      .value = {
                                          .rampTime = OUTPUT_DIMMABLE_RAMP_TIME,
                                          .r = 0,
                                          .g = 0,
                                          .b = 0,
                                      },
                                      .holdTime = OUTPUT_BLINK_PERIOD_OFF + OUTPUT_DIMMABLE_RAMP_TIME,
                                  }}});
      }

      void DimmableRGB::blinkRGBInclusive()
      {
        blinkRGBInclusive(OUTPUT_ITERATE_INFINITE);
      }

      void DimmableRGB::blinkRGBInclusive(unsigned long count)
      {
        reset();
        setSequence({.iterations = count,
                     .sequence = {{
                                      .value = {
                                          .rampTime = OUTPUT_DIMMABLE_RAMP_TIME,
                                          .r = 1,
                                          .g = 0,
                                          .b = 0,
                                      },
                                      .holdTime = OUTPUT_BLINK_PERIOD_ON + OUTPUT_DIMMABLE_RAMP_TIME,
                                  },
                                  {
                                      .value = {
                                          .rampTime = OUTPUT_DIMMABLE_RAMP_TIME,
                                          .r = 0,
                                          .g = 1,
                                          .b = 0,
                                      },
                                      .holdTime = OUTPUT_BLINK_PERIOD_ON + OUTPUT_DIMMABLE_RAMP_TIME,
                                  },
                                  {
                                      .value = {
                                          .rampTime = OUTPUT_DIMMABLE_RAMP_TIME,
                                          .r = 0,
                                          .g = 0,
                                          .b = 1,
                                      },
                                      .holdTime = OUTPUT_BLINK_PERIOD_ON + OUTPUT_DIMMABLE_RAMP_TIME,
                                  }}});
      }

      void DimmableRGB::setOff()
      {
        set({.r = 0,
             .g = 0,
             .b = 0});
      }

      void DimmableRGB::setOn()
      {
        set({.r = 1,
             .g = 1,
             .b = 1});
      }

      void DimmableRGB::setR()
      {
        set({.r = 1,
             .g = 0,
             .b = 0});
      }

      void DimmableRGB::setG()
      {
        set({.r = 0,
             .g = 1,
             .b = 0});
      }

      void DimmableRGB::setB()
      {
        set({.r = 0,
             .g = 0,
             .b = 1});
      }

      void DimmableRGB::update()
      {
        _ramp.update();
        Base::update();
      }

      DimmableRGBWS2812::DimmableRGBWS2812(
          char index,
          ESP32_WS2812 *bus) : DimmableRGB([]() {},
                                           [this, index, bus](DimmableRGBValue value)
                                           {
                                             if (value.rampTime)
                                             {
                                               _ramp.set(value.rampTime, [this, index, bus, value]()
                                                         { bus->setLedColor(
                                                               index,
                                                               gamma(_ramp.getDelta(previousValue.r, value.r)) * 0xff,
                                                               gamma(_ramp.getDelta(previousValue.g, value.g)) * 0xff,
                                                               gamma(_ramp.getDelta(previousValue.b, value.b)) * 0xff); });

                                               return;
                                             }

                                             bus->setLedColor(
                                                 index,
                                                 gamma(value.r) * 0xff,
                                                 gamma(value.g) * 0xff,
                                                 gamma(value.b) * 0xff);
                                           })
      {
      }
    }

  } // section namespace
} // project namespace

#endif
