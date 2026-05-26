#include "./setup.h"

#ifdef IOT_NODE_INDICATORS

namespace IotNode::Services::Indicator
{
#if !defined(IOT_NODE_BOARD_WAVESHARE_ESP32_S3_ZERO) && !defined(IOT_NODE_BOARD_WAVESHARE_ESP32_S3_ETH)
  auto service0 = makeService(&Utils::Indicator::indicator0, 0);

#ifdef IOT_NODE_BOARD_ROOM_SENSOR
  auto service1 = makeService(&Utils::Indicator::indicator1, 1);
  auto service2 = makeService(&Utils::Indicator::indicator2, 2);
#endif
#endif

  void setup()
  {
#if !defined(IOT_NODE_BOARD_WAVESHARE_ESP32_S3_ZERO) && !defined(IOT_NODE_BOARD_WAVESHARE_ESP32_S3_ETH)
    Utils::UDP::instance.addService(&service0);

#ifdef IOT_NODE_BOARD_ROOM_SENSOR
    Utils::UDP::instance.addService(&service1);
    Utils::UDP::instance.addService(&service2);
#endif
#endif
  }
}

#endif
