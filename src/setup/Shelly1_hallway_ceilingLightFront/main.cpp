#ifdef IOT_NODE_ENV_SHELLY1_HALLWAY_CEILINGLIGHTFRONT

#include "./main.h"

namespace IotNode::Setup
{
  void setup()
  {
    ::IotNode::Utils::Keepalive::setup();

#ifdef IOT_NODE_LOGGING
    ::IotNode::Utils::Log::setup();
#endif

    delay(IOT_NODE_LOG_DELAY);

    ::IotNode::Utils::UDP::setup();
    ::IotNode::Utils::Link::setup();

    ::IotNode::Services::Hello::setup();
    ::IotNode::Services::Keepalive::setup();
    ::IotNode::Services::SystemInfo::setup();

    Output::setup();

    {
      auto event0 = ::IotNode::Events::Button::makeEvent(&::IotNode::Utils::UDP::instance, 0);
      Button::button0.setChangeCallback([event0](::IotNode::Utils::Button::Update update)
                                        {
        if (::IotNode::Utils::UDP::instance.isListening() && ::IotNode::Utils::UDP::instance.hasEventPeer()) {
          event0(update);
          return;
        }

        if (update.downChanged && !update.down) {
          ::IotNode::Utils::Keepalive::powerCycleProtection.stop();
          Output::output0.toggle();
        } });
      Button::button0.start();
    }

    Input::setup();

    ::IotNode::Utils::OTA::setup();
  }

  void loop()
  {
    Button::update();
    Input::update();

    ::IotNode::Utils::Keepalive::update();
    ::IotNode::Utils::Link::update();
    ::IotNode::Utils::OTA::update();

    delay(IOT_NODE_MUTLITASKING_DELAY);
  }
}

#endif
