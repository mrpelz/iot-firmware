#include "./main.h"

#ifdef IOT_NODE_INDICATORS

namespace IotNode
{
  namespace Utils
  {

    namespace Indicator
    {
      size_t tmp = sizeof(SequenceItem);

      ClassPin::ClassPin(Config _config)
      {
        config = _config;
      }

      void ClassPin::commit()
      {
        if (!state.wasInitialized)
          return;

        digitalWrite(config.pin, (config.invert ? !state.on : state.on));
      }

      bool ClassPin::isOn()
      {
        return state.on;
      }

      void ClassPin::blink()
      {
#ifdef IOT_NODE_LOGGING
        Log::debug("indicator.blink: infinite");
        Log::debug(fmt::format("indicator.pin: {}", config.pin));
#endif

        state.on = false;

        state.blinkChange = millis();
        state.blinkCount = 0;
        state.blinkInfinite = true;
      }

      void ClassPin::blink(uint8_t count)
      {
#ifdef IOT_NODE_LOGGING
        Log::debug(fmt::format("indicator.blink: {}", count));
        Log::debug(fmt::format("indicator.pin: {}", config.pin));
#endif

        state.on = false;

        state.blinkChange = millis();
        state.blinkCount = count * 2;
        state.blinkInfinite = false;
      }

      void ClassPin::init()
      {
#ifdef IOT_NODE_LOGGING
        Log::debug("indicator: init");
        Log::debug(fmt::format("indicator.pin: {}", config.pin));
#endif

        pinMode(config.pin, OUTPUT);
        state.wasInitialized = true;

        commit();
      }

      void ClassPin::setBlinkFrequency(unsigned long blinkPeriodOn, unsigned long blinkPeriodOff)
      {
        config.blinkPeriodOn = blinkPeriodOn;
        config.blinkPeriodOff = blinkPeriodOff;
      }

      void ClassPin::setOn(bool on)
      {
#ifdef IOT_NODE_LOGGING
        Log::debug(fmt::format("indicator.set-on: {}", on ? "on" : "off"));
        Log::debug(fmt::format("indicator.pin: {}", config.pin));
#endif

        state.on = on;

        // abort blink
        state.blinkCount = 0;
        state.blinkInfinite = false;

        commit();
      }

      void ClassPin::toggle()
      {
#ifdef IOT_NODE_LOGGING
        Log::debug(fmt::format("indicator.toggle: {}", state.on ? "on2off" : "off2on"));
        Log::debug(fmt::format("indicator.pin: {}", config.pin));
#endif

        state.on = !state.on;

        // abort blink
        state.blinkCount = 0;
        state.blinkInfinite = false;

        commit();
      }

      void ClassPin::update()
      {
        if (!state.blinkCount && !state.blinkInfinite)
          return;

        unsigned long now = millis();
        unsigned long timeSinceLastBlink = now - state.blinkChange;
        unsigned long relevantBlinkPeriod = state.on
                                                ? config.blinkPeriodOn
                                                : config.blinkPeriodOff;

        if (timeSinceLastBlink < relevantBlinkPeriod)
          return;

        state.blinkChange = now;
        state.on = !state.on;

        if (!state.blinkInfinite)
        {
          state.blinkCount = state.blinkCount - 1;
        }

        commit();
      }

#ifdef IOT_NODE_SX1509

      ClassExpander::ClassExpander(Config _config)
      {
        config = _config;
      }

      void ClassExpander::commit()
      {
        if (!state.wasInitialized)
          return;

        auto effectiveOn = config.invert ? !state.on : state.on;

        if (effectiveOn)
        {
          Sx1509::io.pinMode(config.pin, INPUT);
          return;
        }

        Sx1509::io.pinMode(config.pin, OUTPUT);
      }

      bool ClassExpander::isOn()
      {
        return state.on;
      }

      void ClassExpander::blink()
      {
#ifdef IOT_NODE_LOGGING
        Log::debug("indicator.blink: infinite");
        Log::debug(fmt::format("indicator.pin: {}", config.pin));
#endif

        state.on = false;

        state.blinkChange = millis();
        state.blinkCount = 0;
        state.blinkInfinite = true;
      }

      void ClassExpander::blink(uint8_t count)
      {
#ifdef IOT_NODE_LOGGING
        Log::debug(fmt::format("indicator.blink: {}", count));
        Log::debug(fmt::format("indicator.pin: {}", config.pin));
#endif

        state.on = false;

        state.blinkChange = millis();
        state.blinkCount = count * 2;
        state.blinkInfinite = false;
      }

      void ClassExpander::init()
      {
#ifdef IOT_NODE_LOGGING
        Log::debug("indicator: init");
        Log::debug(fmt::format("indicator.pin: {}", config.pin));
#endif

        state.wasInitialized = true;

        commit();
      }

      void ClassExpander::setBlinkFrequency(unsigned long blinkPeriodOn, unsigned long blinkPeriodOff)
      {
        config.blinkPeriodOn = blinkPeriodOn;
        config.blinkPeriodOff = blinkPeriodOff;
      }

      void ClassExpander::setOn(bool on)
      {
#ifdef IOT_NODE_LOGGING
        Log::debug(fmt::format("indicator.set-on: {}", on ? "on" : "off"));
        Log::debug(fmt::format("indicator.pin: {}", config.pin));
#endif

        state.on = on;

        // abort blink
        state.blinkCount = 0;
        state.blinkInfinite = false;

        commit();
      }

      void ClassExpander::toggle()
      {
#ifdef IOT_NODE_LOGGING
        Log::debug(fmt::format("indicator.toggle: {}", state.on ? "on2off" : "off2on"));
        Log::debug(fmt::format("indicator.pin: {}", config.pin));
#endif

        state.on = !state.on;

        // abort blink
        state.blinkCount = 0;
        state.blinkInfinite = false;

        commit();
      }

      void ClassExpander::update()
      {
        if (!state.blinkCount && !state.blinkInfinite)
          return;

        unsigned long now = millis();
        unsigned long timeSinceLastBlink = now - state.blinkChange;
        unsigned long relevantBlinkPeriod = state.on
                                                ? config.blinkPeriodOn
                                                : config.blinkPeriodOff;

        if (timeSinceLastBlink < relevantBlinkPeriod)
          return;

        state.blinkChange = now;
        state.on = !state.on;

        if (!state.blinkInfinite)
        {
          state.blinkCount = state.blinkCount - 1;
        }

        commit();
      }

#endif
    }

  } // section namespace
} // project namespace

#endif
