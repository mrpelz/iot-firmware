#ifdef IOT_NODE_ENV_STORAGEROOM_WASHER_DRYER_BRIDGE

#include "./main.h"

namespace IotNode::Setup
{
  void setup()
  {
    ::IotNode::Utils::Keepalive::setup();

#ifdef IOT_NODE_LOGGING
    ::IotNode::Utils::Log::setup();
#endif

    vTaskDelay(pdMS_TO_TICKS(100));

    ::IotNode::Utils::UDP::setup();
    ::IotNode::Utils::Link::setup();

    ::IotNode::Services::Hello::setup();
    ::IotNode::Services::Keepalive::setup();
    ::IotNode::Services::SystemInfo::setup();

    TCPBridge::setup();

    ::IotNode::Utils::OTA::setup();
  }

  void loop()
  {
    ::IotNode::Utils::OTA::update();

    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

#endif
