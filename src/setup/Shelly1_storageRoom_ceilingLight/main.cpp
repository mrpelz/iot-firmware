#ifdef IOT_NODE_ENV_SHELLY1_STORAGEROOM_CEILINGLIGHT

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
    Button::setup();
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
