#ifdef IOT_NODE_ENV_WAVESHARE_ESP32_S3_ETH

#include "./main.h"

namespace IotNode::Setup
{
  void setup()
  {
    ::IotNode::Utils::Keepalive::setup();

#ifdef IOT_NODE_LOGGING
    ::IotNode::Utils::Log::setup();
#endif

    vTaskDelay(IOT_NODE_LOG_DELAY / portTICK_PERIOD_MS);

    ::IotNode::Utils::UDP::setup();
    ::IotNode::Utils::Link::setup();

    ::IotNode::Services::Hello::setup();
    ::IotNode::Services::Keepalive::setup();
    ::IotNode::Services::SystemInfo::setup();

    OutputNg::setup();

    ::IotNode::Utils::OTA::setup();
  }

  void loop()
  {
    ::IotNode::Utils::OTA::update();
    vTaskDelay(IOT_NODE_MUTLITASKING_DELAY / portTICK_PERIOD_MS);
  }
}

#endif
