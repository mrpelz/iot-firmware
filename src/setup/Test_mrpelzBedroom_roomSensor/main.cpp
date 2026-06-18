#ifdef IOT_NODE_BOARD_ROOM_SENSOR

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

    ::IotNode::Utils::I2C::setup();
    ::IotNode::Utils::Sx1509::setup();
    ::IotNode::Utils::UDP::setup();
    ::IotNode::Utils::Link::setup();

    ::IotNode::Services::Hello::setup();
    ::IotNode::Services::Keepalive::setup();
    ::IotNode::Services::SystemInfo::setup();

    Indicator::setup();

    ::IotNode::Services::Bme280::setup();
    ::IotNode::Services::Mcp9808::setup();
    ::IotNode::Services::Sgp30::setup();
    ::IotNode::Services::Tsl2561::setup();

    ::IotNode::Utils::OTA::setup();

    if (::IotNode::Utils::Link::link.isConnected())
      return;

    Indicator::indicator0.blink();
  }

  void loop()
  {
    ::IotNode::Utils::OTA::update();

    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

#endif
