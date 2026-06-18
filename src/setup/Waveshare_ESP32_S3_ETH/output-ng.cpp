#ifdef IOT_NODE_BOARD_WAVESHARE_ESP32_S3_ETH

#include "./output-ng.h"

namespace IotNode::Setup::OutputNg
{
  ESP32_WS2812 ws2812Bus0 = ESP32_WS2812(1, 21, 0, TYPE_GRB);
  ::IotNode::Utils::OutputNg::DimmableRGBWS2812 indicatorRGB(0, &ws2812Bus0, false);

  auto service0 = ::IotNode::Services::OutputNg::makeService(&indicatorRGB, 1);

  void update()
  {
    indicatorRGB.update();
    ws2812Bus0.show();
  }

  void task(void *parameter)
  {
    for (;;)
    {
      update();
      vTaskDelay(pdMS_TO_TICKS(100));
    }
  }

  void setup()
  {
    ws2812Bus0.begin();
    indicatorRGB.init();
    indicatorRGB.blinkRGBInclusive();

    ::IotNode::Utils::UDP::instance.addService(&service0);

    xTaskCreatePinnedToCore(
        task,
        "output_ng_maintenance",
        FREERTOS_STACK_SIZE,
        NULL,
        tskIDLE_PRIORITY,
        NULL,
        CONFIG_ARDUINO_RUNNING_CORE);
  }
}

#endif
