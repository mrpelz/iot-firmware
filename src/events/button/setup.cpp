#include "./setup.h"

#ifdef IOT_NODE_BUTTONS

namespace IotNode
{
  namespace Events
  {

    namespace Button
    {
      void setup()
      {
        auto event0 = makeEvent(&Utils::UDP::instance, 0);

#ifdef IOT_NODE_OUTPUT
        Utils::Button::button0.setChangeCallback([event0](Utils::Button::Update update)
                                                 {
        if (Utils::UDP::instance.isListening() && Utils::UDP::instance.hasEventPeer()) {
          event0(update);
          return;
        }

#ifdef IOT_NODE_LOGGING
          Utils::Log::debug("info.buttons.change-callback", "udp event not usable");
#endif

        if (update.downChanged && !update.down) {
#ifdef IOT_NODE_LOGGING
            Utils::Log::debug("info.buttons.change-callback", "triggering override");
#endif

#ifdef IOT_NODE_POWER_CYCLE_PROTECTION
            Utils::Keepalive::powerCycleProtection.stop();
#endif
          Utils::Output::output0.toggle();
        } });
#else
        Utils::Button::button0.setChangeCallback(event0);
#endif

#if defined(IOT_NODE_BOARD_SHELLYI3) || defined(IOT_NODE_BOARD_ESP_NOW_TEST_BUTTON)
        auto event1 = makeEvent(&Utils::UDP::instance, 1);
        Utils::Button::button1.setChangeCallback(event1);
#endif

#ifdef IOT_NODE_BOARD_SHELLYI3

        auto event2 = makeEvent(&Utils::UDP::instance, 2);
        Utils::Button::button2.setChangeCallback(event2);
#endif
      }

#ifdef IOT_NODE_ESP_NOW_NODE
      void setupEspNow()
      {
        auto event0 = makeEspNowEvent(0);

#ifdef IOT_NODE_BOARD_ESP_NOW_TEST_BUTTON
        auto event1 = makeEspNowEvent(1);
#endif

        Utils::Button::button0.setChangeCallback(event0);

#ifdef IOT_NODE_BOARD_ESP_NOW_TEST_BUTTON
        Utils::Button::button1.setChangeCallback(event1);
#endif

        Utils::Button::button0.update();

#ifdef IOT_NODE_BOARD_ESP_NOW_TEST_BUTTON
        Utils::Button::button1.update();
#endif
      }
#endif
    }

  } // section namespace
} // project namespace

#endif
