#include "./main.h"

#ifdef IOT_NODE_OUTPUT

namespace IotNode
{
  namespace Utils
  {

    namespace Output
    {
      Regular::Regular(RegularConfig _config)
      {
        config = _config;
      }

      void Regular::commit()
      {
        if (!state.wasInitialized)
          return;

#ifdef IOT_NODE_LOGGING
        Log::debug("output", "commit");
#endif

        digitalWrite(config.pin, (config.invert ? !state.on : state.on));
      }

      bool Regular::isOn()
      {
        return state.on;
      }

      void Regular::init()
      {
#ifdef IOT_NODE_LOGGING
        Log::debug("output", "init");
        Log::debug("output.pin", String(config.pin));
#endif

        pinMode(config.pin, OUTPUT);
        state.wasInitialized = true;

        commit();
      }

      void Regular::setOn(bool on)
      {
#ifdef IOT_NODE_LOGGING
        Log::debug("output.set-on", on ? "on" : "off");
        Log::debug("output.pin", String(config.pin));
#endif

        state.on = on;
        commit();
      }

      void Regular::toggle()
      {
#ifdef IOT_NODE_LOGGING
        Log::debug("output.toggle", state.on ? "on2off" : "off2on");
        Log::debug("output.pin", String(config.pin));
#endif

        state.on = !state.on;
        commit();
      }

      Pulse::Pulse(PulseConfig _config)
      {
        config = _config;
      }

      void Pulse::commit()
      {
        if (!state.wasInitialized)
          return;

#ifdef IOT_NODE_LOGGING
        Log::debug("output", "commit");
#endif

        auto unactionablePin = state.on ? config.offPin : config.onPin;
        auto actionablePin = state.on ? config.onPin : config.offPin;

        digitalWrite(unactionablePin, config.invert);
        digitalWrite(actionablePin, !config.invert);
      }

      bool Pulse::isOn()
      {
        return state.on;
      }

      void Pulse::init()
      {
#ifdef IOT_NODE_LOGGING
        Log::debug("output", "init");
        Log::debug("output.on-pin", String(config.onPin));
        Log::debug("output.off-pin", String(config.offPin));
#endif

        pinMode(config.onPin, OUTPUT);
        pinMode(config.offPin, OUTPUT);
        state.wasInitialized = true;

        commit();
      }

      void Pulse::setOn(bool on)
      {
#ifdef IOT_NODE_LOGGING
        Log::debug("output.set-on", on ? "on" : "off");
        Log::debug("output.on-pin", String(config.onPin));
        Log::debug("output.off-pin", String(config.offPin));
#endif

        state.on = on;
        commit();
      }

      void Pulse::toggle()
      {
#ifdef IOT_NODE_LOGGING
        Log::debug("output.toggle", state.on ? "on2off" : "off2on");
        Log::debug("output.on-pin", String(config.onPin));
        Log::debug("output.off-pin", String(config.offPin));
#endif

        state.on = !state.on;
        commit();
      }
    }

  } // section namespace
} // project namespace

#endif
