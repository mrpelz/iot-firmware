#include "./setup.h"

#if defined(IOT_NODE_INDICATORS) && defined(IOT_NODE_BOARD_WAVESHARE_ESP32_S3_ZERO)

namespace IotNode::Services::OutputNg
{
  auto service0 = makeService(&Utils::OutputNg::indicator0, 0);
  auto service1 = makeService(&Utils::OutputNg::indicator1, 0);
  auto service2 = makeService(&Utils::OutputNg::indicatorRGB, 1);
  auto service3 = makeService(&Utils::OutputNg::buzzer, 0);

  void setup()
  {
    Utils::UDP::instance.addService(&service0);
    Utils::UDP::instance.addService(&service1);
    Utils::UDP::instance.addService(&service2);
    Utils::UDP::instance.addService(&service3);
  }
}

#endif
