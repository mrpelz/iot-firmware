#include "./main.h"

#ifdef IOT_NODE_OUTPUT_NG

namespace IotNode::Utils::OutputNg
{
  double gamma(double input, double gamma = OUTPUT_DIMMABLE_GAMMA)
  {
    return pow(input, gamma);
  }

  template <typename T>
  Base<T>::Base(
      T initialValue,
      ::std::function<void(T value)> onCommit,
      ::std::function<void()> onInit)
  {
    value = initialValue;
    previousValue = initialValue;

    _onCommit = onCommit;
    _onInit = onInit;

    _reset();
  }

  template <typename T>
  void Base<T>::_commit()
  {
    if (!_state.isInitialized)
      return;

    if (_onCommit == NULL)
      return;

    _onCommit(value);
  }

  template <typename T>
  bool Base<T>::getIsActive()
  {
    return _state.iterations;
  }

  template <typename T>
  void Base<T>::init()
  {
    if (_onInit == NULL)
      return;

    _onInit();
    _state.isInitialized = true;
  }

  template <typename T>
  void Base<T>::setSequenceCallbacks(::std::function<void(unsigned long remainingIterations)> onIterate, ::std::function<void()> onReset)
  {
    _onIterate = onIterate;
    _onReset = onReset;
  }

  template <typename T>
  void Base<T>::set(T value_)
  {
    _reset();
    previousValue = value;
    value = value_;

    _commit();
  }

  template <typename T>
  void Base<T>::setSequence(Request<T> request, ::std::function<void(unsigned long remainingIterations)> onIterateCb, ::std::function<void()> onResetCb)
  {
    _reset();

    _state.iterations = request.iterations;
    _state.sequence = request.sequence;

    setSequenceCallbacks(onIterateCb, onResetCb);
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
      _reset();
      return;
    }

#ifdef IOT_NODE_LOGGING
    Log::debug(fmt::format("update.sequenceSize: {}", _state.sequence.size()));
    Log::debug(fmt::format("update.iterations: {}", _state.iterations));
    Log::debug(fmt::format("update.sequencePointer: {}", _state.sequencePointer));
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

      if (_onIterate != NULL)
      {
        _onIterate(_state.iterations);
      }

      if (!_state.iterations)
      {
        _reset();
      }

      _state.sequencePointer = 0;
    }

    _commit();
  }

  template <typename T>
  void Base<T>::_reset()
  {
    _state.iterations = 0;
    _state.nextSequenceStep = 0;
    _state.sequence.clear();
    _state.sequencePointer = 0;

    if (_onReset != NULL)
    {
      _onReset();
    }

    _onIterate = NULL;
    _onReset = NULL;
  }

  Ramp::Ramp(unsigned long throttle = OUTPUT_RAMP_THROTTLE)
  {
    _throttle = throttle;
  }

  double Ramp::getDelta(double startValue, double endValue)
  {
    auto delta = endValue - startValue;
    return startValue + (delta * progress);
  }

  unsigned long Ramp::getDelta(unsigned long startValue, unsigned long endValue)
  {
    signed long delta = endValue - startValue;
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

  void Ramp::set(unsigned long duration, ::std::function<void()> onUpdate)
  {
    _duration = duration;
    _onUpdate = onUpdate;
    _startTime = millis();
  }

  void Ramp::update()
  {
    if (!_startTime || !_duration || !_onUpdate)
      return;

    auto now = millis();

    if (_throttle)
    {
      auto timeSinceLastUpdate = now - _lastUpdate;
      if (timeSinceLastUpdate < _throttle)
        return;
    }

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

  template class Base<DimmableValue<unsigned long>>;
  Buzzer::Buzzer(
      unsigned char pin,
      bool invert) : Base<DimmableValue<unsigned long>>({.rampTime = 0, .value = 0}, [this](DimmableValue<unsigned long> value)
                                                        {
                                                                                                 if (value.rampTime)
                                                                                                 {
                                                                                                   _ramp.set(value.rampTime, [this, value]()
                                                                                                             { _write(_ramp.getDelta(
                                                                                                                   previousValue.value,
                                                                                                                   value.value)); });

                                                                                                   return;
                                                                                                 }

                                                                                                 _write(value.value); })
  {
    _invert = invert;
    _pin = pin;
    _ramp = Ramp(0);
  }

  void Buzzer::_write(unsigned long value)
  {
    if (value)
    {
      if (!_isAttached)
      {
#ifdef IOT_NODE_ESP32
        ledcAttachChannel(
            _pin,
            OUTPUT_BUZZER_DEFAULT_FREQUENCY,
            OUTPUT_BUZZER_LEDC_RESOLUTION,
            OUTPUT_BUZZER_LEDC_CHANNEL);
        ledcOutputInvert(_pin, _invert);
#endif
#ifdef IOT_NODE_ESP8266
        constexpr int pwm = outputDimmableFull / 2;
        analogWrite(_pin, pwm);
#endif

        _isAttached = true;
      }
#ifdef IOT_NODE_ESP32
      ledcWriteTone(_pin,
                    value);
#endif
#ifdef IOT_NODE_ESP8266
      analogWriteFreq(value);
#endif

      return;
    }

    if (_isAttached)
    {
#ifdef IOT_NODE_ESP32
      ledcDetach(_pin);
#endif
#ifdef IOT_NODE_ESP8266
      analogWrite(_pin, 0);
#endif

      _isAttached = false;
    }

    pinMode(_pin, INPUT);
  }

  void Buzzer::beep()
  {
    beep(OUTPUT_ITERATE_INFINITE);
  }

  void Buzzer::beep(unsigned long count)
  {
    beep(count, OUTPUT_BUZZER_DEFAULT_FREQUENCY);
  }

  void Buzzer::beep(unsigned long count, unsigned long frequency)
  {
    setSequence({.iterations = count,
                 .sequence = {
                     {
                         .value = {
                             .rampTime = 0,
                             .value = frequency,
                         },
                         .holdTime = OUTPUT_BUZZER_HOLD_TIME,
                     },
                     {
                         .value = {
                             .rampTime = 0,
                             .value = 0,
                         },
                         .holdTime = OUTPUT_BUZZER_PAUSE_TIME,
                     }}});
  }

  void Buzzer::melody(
      ::std::vector<unsigned long> _melody)
  {
    melody(_melody, 1);
  }

  void Buzzer::melody(
      ::std::vector<unsigned long> _melody,
      unsigned long count)
  {
    melody(_melody, count, OUTPUT_BUZZER_HOLD_TIME, OUTPUT_BUZZER_PAUSE_TIME);
  }

  void Buzzer::melody(
      ::std::vector<unsigned long> _melody,
      unsigned long count,
      unsigned long holdTime,
      unsigned long pauseTime)
  {
    ::std::vector<SequenceItem<DimmableValue<unsigned long>>> sequence(_melody.size());

    for (auto &item : _melody)
    {
      if (holdTime)
      {
        sequence.push_back({
            .value = {
                .rampTime = 0,
                .value = item,
            },
            .holdTime = holdTime,
        });
      }

      if (pauseTime)
      {
        sequence.push_back({
            .value = {
                .rampTime = 0,
                .value = 0,
            },
            .holdTime = pauseTime,
        });
      }
    }

    if (!pauseTime)
    {
      sequence.push_back({
          .value = {
              .rampTime = 0,
              .value = 0,
          },
          .holdTime = 0,
      });
    }

    setSequence({.iterations = count,
                 .sequence = sequence});
  }

  void Buzzer::update()
  {
    _ramp.update();
    Base::update();
  }

  template class Base<bool>;
  Binary::Binary(
      unsigned char pin,
      bool invert) : Base<bool>(false, [pin, invert](bool value)
                                { digitalWrite(pin, (invert ? !value : value)); }, [pin]()
                                { pinMode(pin, OUTPUT); })
  {
  }
  Binary::Binary(
      ::std::function<void(bool value)> onCommit,
      ::std::function<void()> onInit) : Base<bool>(false,
                                                   onCommit,
                                                   onInit)
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
      unsigned char pinOn,
      unsigned char pinOff,
      bool invert,
      unsigned char pulseTime = 50) : Binary([pinOn, pinOff, invert, pulseTime](bool value)
                                             {
          digitalWrite(value ? pinOn : pinOff, !invert);
          delay(pulseTime);

          digitalWrite(value ? pinOn : pinOff, invert); },
                                             [pinOn, pinOff]()
                                             {
                                  pinMode(pinOn, OUTPUT);
                                  pinMode(pinOff, OUTPUT); }) {}

  template class Base<DimmableValue<double>>;
  Dimmable::Dimmable(
      unsigned char pin,
      bool invert) : Base<DimmableValue<double>>({.rampTime = 0, .value = 0}, [this, invert, pin](DimmableValue<double> value)
                                                 {
                                                   if (value.rampTime)
                                                   {
                                                     _ramp.set(value.rampTime, [this, invert, pin, value]()
                                                               {
#ifdef IOT_NODE_ESP32
                                                                 ledcWrite(pin, gamma(_ramp.getDelta(previousValue.value, value.value)) * outputDimmableFull);
#endif
#ifdef IOT_NODE_ESP8266
                                                                 int pwm = gamma(_ramp.getDelta(previousValue.value, value.value)) * outputDimmableFull;
                                                                 analogWrite(pin, invert ? outputDimmableFull - pwm : pwm);
#endif
                                                               });

                                                     return;
                                                   }
#ifdef IOT_NODE_ESP32
                                                   ledcWrite(pin, gamma(value.value) * outputDimmableFull);
#endif
#ifdef IOT_NODE_ESP8266
                                                   int pwm = gamma(value.value) * outputDimmableFull;
                                                   analogWrite(pin, invert ? outputDimmableFull - pwm : pwm);
#endif
                                                 },
                                                 [pin, invert]()
                                                 {
#ifdef IOT_NODE_ESP32
                                                   ledcAttach(pin, OUTPUT_DIMMABLE_FREQUENCY, OUTPUT_DIMMABLE_RESOLUTION);
                                                   ledcOutputInvert(pin, invert);
#endif
#ifdef IOT_NODE_ESP8266
                                                   analogWriteResolution(OUTPUT_DIMMABLE_RESOLUTION);
#endif
                                                 })
  {
    _ramp = Ramp();
  }
  Dimmable::Dimmable(
      ::std::function<void(DimmableValue<double> value)> onCommit,
      ::std::function<void()> onInit) : Base<DimmableValue<double>>({.rampTime = 0,
                                                                     .value = 0},
                                                                    onCommit,
                                                                    onInit)
  {
    _ramp = Ramp();
  }

  void Dimmable::blink()
  {
    blink(OUTPUT_ITERATE_INFINITE);
  }

  void Dimmable::blink(unsigned long count)
  {
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
  DimmableRGB::DimmableRGB(
      unsigned char pinR,
      unsigned char pinG,
      unsigned char pinB,
      bool invert) : Base<DimmableRGBValue>({.rampTime = 0, .r = 0, .g = 0, .b = 0}, [this, invert, pinR, pinG, pinB](DimmableRGBValue value)
                                            {
                                              if (value.rampTime)
                                              {
                                                _ramp.set(value.rampTime, [this, invert, pinR, pinG, pinB, value]()
                                                          {
#ifdef IOT_NODE_ESP32
                                                            ledcWrite(pinR, gamma(_ramp.getDelta(previousValue.r, value.r)) * outputDimmableFull);
                                                            ledcWrite(pinG, gamma(_ramp.getDelta(previousValue.g, value.g)) * outputDimmableFull);
                                                            ledcWrite(pinB, gamma(_ramp.getDelta(previousValue.b, value.b)) * outputDimmableFull);
#endif
#ifdef IOT_NODE_ESP8266
                                                            int pwmR = gamma(_ramp.getDelta(previousValue.r, value.r)) * outputDimmableFull;
                                                            analogWrite(pinR, invert ? outputDimmableFull - pwmR : pwmR);

                                                            int pwmG = gamma(_ramp.getDelta(previousValue.g, value.g)) * outputDimmableFull;
                                                            analogWrite(pinG, invert ? outputDimmableFull - pwmG : pwmG);

                                                            int pwmB = gamma(_ramp.getDelta(previousValue.b, value.b)) * outputDimmableFull;
                                                            analogWrite(pinB, invert ? outputDimmableFull - pwmB : pwmB);
#endif
                                                          });

                                                return;
                                              }
#ifdef IOT_NODE_ESP32
                                              ledcWrite(pinR, gamma(value.r) * outputDimmableFull);
                                              ledcWrite(pinG, gamma(value.g) * outputDimmableFull);
                                              ledcWrite(pinB, gamma(value.b) * outputDimmableFull);
#endif
#ifdef IOT_NODE_ESP8266
                                              int pwmR = gamma(value.r) * outputDimmableFull;
                                              analogWrite(pinR, invert ? outputDimmableFull - pwmR : pwmR);

                                              int pwmG = gamma(value.g) * outputDimmableFull;
                                              analogWrite(pinG, invert ? outputDimmableFull - pwmG : pwmG);

                                              int pwmB = gamma(value.b) * outputDimmableFull;
                                              analogWrite(pinB, invert ? outputDimmableFull - pwmB : pwmB);
#endif
                                            },
                                            [pinR, pinG, pinB, invert]()
                                            {
#ifdef IOT_NODE_ESP32
                                              ledcAttach(pinR, OUTPUT_DIMMABLE_FREQUENCY, OUTPUT_DIMMABLE_RESOLUTION);
                                              ledcOutputInvert(pinR, invert);

                                              ledcAttach(pinG, OUTPUT_DIMMABLE_FREQUENCY, OUTPUT_DIMMABLE_RESOLUTION);
                                              ledcOutputInvert(pinG, invert);

                                              ledcAttach(pinB, OUTPUT_DIMMABLE_FREQUENCY, OUTPUT_DIMMABLE_RESOLUTION);
                                              ledcOutputInvert(pinB, invert);
#endif
#ifdef IOT_NODE_ESP8266
                                              analogWriteResolution(OUTPUT_DIMMABLE_RESOLUTION);
#endif
                                            })
  {
    _ramp = Ramp();
  }
  DimmableRGB::DimmableRGB(
      ::std::function<void(DimmableRGBValue value)> onCommit,
      ::std::function<void()> onInit) : Base<DimmableRGBValue>({.rampTime = 0,
                                                                .r = 0,
                                                                .g = 0,
                                                                .b = 0},
                                                               onCommit,
                                                               onInit)
  {
    _ramp = Ramp();
  }

  void DimmableRGB::blink()
  {
    blink(OUTPUT_ITERATE_INFINITE);
  }

  void DimmableRGB::blink(unsigned long count)
  {
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

#ifdef IOT_NODE_WS2812
  DimmableRGBWS2812::DimmableRGBWS2812(
      unsigned char index,
      ESP32_WS2812 *bus,
      bool push) : DimmableRGB([this, index, bus, push](DimmableRGBValue value)
                               {
          if (value.rampTime)
          {
            _ramp.set(value.rampTime, [this, index, bus, push, value]()
                      { bus->setLedColorData(
                                       index,
                                       gamma(_ramp.getDelta(previousValue.r, value.r)) * 0xff,
                                       gamma(_ramp.getDelta(previousValue.g, value.g)) * 0xff,
                                       gamma(_ramp.getDelta(previousValue.b, value.b)) * 0xff);

                                       if (push) bus->show(); });

            return;
          }

          bus->setLedColorData(index,
                               gamma(value.r) * 0xff,
                               gamma(value.g) * 0xff,
                               gamma(value.b) * 0xff);

          if (push)
            bus->show(); })
  {
  }
#endif
}

#endif
