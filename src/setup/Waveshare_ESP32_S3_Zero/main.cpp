#ifdef IOT_NODE_ENV_WAVESHARE_ESP32_S3_ZERO

#include "./main.h"

namespace IotNode::Setup
{

  void setup()
  {
    ::IotNode::Utils::Keepalive::setup();

#ifdef IOT_NODE_LOGGING
    ::IotNode::Utils::Log::setup();
#endif

    vTaskDelay(pdMS_TO_TICKS(IOT_NODE_MUTLITASKING_DELAY));

    ::IotNode::Utils::Link::setup();
    ::IotNode::Utils::OTA::setup();

    if (::IotNode::Utils::SafeMode::isSafeMode())
      return;

    ::IotNode::Utils::UDP::setup();

    ::IotNode::Services::Hello::setup();
    ::IotNode::Services::Keepalive::setup();
    ::IotNode::Services::SystemInfo::setup();

    OutputNg::setup();
  }

  void loop()
  {
    ::IotNode::Utils::OTA::update();

    vTaskDelay(pdMS_TO_TICKS(IOT_NODE_MUTLITASKING_DELAY));
  }
}

#endif
