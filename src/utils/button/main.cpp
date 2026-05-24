#include "./main.h"

#ifdef IOT_NODE_BUTTONS

namespace IotNode
{
  namespace Utils
  {

    namespace Button
    {
      ::std::vector<Class *> buttons;

      Class::Class(Config _config)
      {
        config = _config;
      }

      bool Class::isDown()
      {
        return state.down;
      }

      void Class::setChangeCallback(ChangeCallback callback)
      {
        state.changeCallback = callback;
      }

      void Class::start()
      {
        buttons.insert(buttons.end(), this);

        state.running = true;

        if (config.pin == 0xff)
          return;

        pinMode(
            config.pin,
            config.pullupEnable ? INPUT_PULLUP : INPUT);
      }

      void Class::stop()
      {
        state.running = false;

        if (config.pin == 0xff)
          return;

        pinMode(
            config.pin,
            INPUT);
      }

      void Class::update()
      {
        update(true);
      }
      void Class::update(bool insert)
      {
        if (!state.running)
          return;

        auto now = millis();

        bool rawDown = config.pin == 0xff ? insert : digitalRead(config.pin);
        auto down = config.invert ? rawDown : !rawDown;

        if (config.noiseGateTime)
        {
          if (!state.noiseGateTime && down)
          {
            state.noiseGateTime = now;
            return;
          }

          auto timeSinceNoiseGateBegin = now - state.noiseGateTime;
          if (timeSinceNoiseGateBegin < config.noiseGateTime && down)
          {
            return;
          }

          if (state.noiseGateTime && !down)
          {
            state.noiseGateTime = 0;
            return;
          }

          state.noiseGateTime = 0;
        }

        auto timeSinceLastChange = now - state.changeTime;
        if (timeSinceLastChange < config.debounceTime)
          return;

        bool downChanged = down != state.down;
        auto longpressChanged = false;

        if (!down || (down && downChanged))
        {
          state.longpress = 0;
          state.longpressTime = now;
        }
        else
        {
          auto timeSinceLastLongpress = now - state.longpressTime;

          if (timeSinceLastLongpress > config.longpressTime)
          {
            longpressChanged = true;
            state.longpress = state.longpress + 1;
            state.longpressTime = now;
          }
        }

        if (downChanged)
        {
          auto lastChangeTime = state.changeTime;

          state.changeTime = now;
          state.down = down;

          if (down && lastChangeTime)
          {
            if (timeSinceLastChange < config.repeatTime)
            {
              state.repeat = state.repeat + 1;
            }
            else
            {
              state.repeat = 0;
            }
          }
        }

        if (!(downChanged || longpressChanged))
          return;

        ::std::vector<bool> pressedMap;
        ::std::for_each(
            ::std::begin(buttons),
            ::std::end(buttons),
            [&pressedMap](Class *button)
            {
              pressedMap.insert(pressedMap.end(), button->isDown());
            });

#ifdef IOT_NODE_LOGGING
        Log::debug("event: button");
        Log::debug(fmt::format("button.down: {}", state.down));
        Log::debug(fmt::format("button.down.changed: {}", downChanged));
        Log::debug(fmt::format("button.down.change-period: {}", timeSinceLastChange));
        Log::debug(fmt::format("button.repeat: {}", state.repeat));
        Log::debug(fmt::format("button.longpress: {}", state.longpress));
#endif

        state.changeCallback({state.down,
                              downChanged,
                              timeSinceLastChange,
                              state.repeat,
                              state.longpress,
                              pressedMap});
      }
    }

  } // section namespace
} // project namespace

#endif
