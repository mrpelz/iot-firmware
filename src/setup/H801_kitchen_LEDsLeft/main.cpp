#ifdef IOT_NODE_ENV_H801_KITCHEN_LEDSLEFT

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

    Indicator::setup();
    Led::setup();

    ::IotNode::Utils::UDP::setup();
    ::IotNode::Utils::Link::setup();

    ::IotNode::Services::Hello::setup();
    ::IotNode::Services::Keepalive::setup();
    ::IotNode::Services::SystemInfo::setup();

    ::IotNode::Utils::OTA::setup();

    if (::IotNode::Utils::Link::link.isConnected())
      return;

    Indicator::indicator0.blink();
  }

  void loop()
  {
    Indicator::update();
    Led::update();

    ::IotNode::Utils::Keepalive::update();
    ::IotNode::Utils::Link::update();
    ::IotNode::Utils::OTA::update();

    delay(IOT_NODE_MUTLITASKING_DELAY);
  }
}

#endif
