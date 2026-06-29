#ifdef IOT_NODE_BOARD_WAVESHARE_ESP32_S3_ZERO

#include "./output-ng.h"

namespace IotNode::Setup::OutputNg
{

  ::IotNode::Utils::OutputNg::Dimmable indicator0(1, true);
  auto service0 = ::IotNode::Services::OutputNg::makeService(&indicator0, &::IotNode::Utils::UDP::instance, 0);

  ::IotNode::Utils::OutputNg::DimmableRGB indicator1(2, 3, 4, false);
  auto service1 = ::IotNode::Services::OutputNg::makeService(&indicator1, &::IotNode::Utils::UDP::instance, 0);

  ::IotNode::Utils::OutputNg::Buzzer buzzer(5, true);
  auto service3 = ::IotNode::Services::OutputNg::makeService(&buzzer, &::IotNode::Utils::UDP::instance, 0);

  ESP32_WS2812 ws2812Bus0 = ESP32_WS2812(1, 21, 0, TYPE_RGB);
  ::IotNode::Utils::OutputNg::DimmableRGBWS2812 indicatorRGB(0, &ws2812Bus0, false);
  auto service2 = ::IotNode::Services::OutputNg::makeService(&indicatorRGB, &::IotNode::Utils::UDP::instance, 1);

  void update()
  {
    indicator0.update();

    indicator1.update();

    buzzer.update();

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
    indicator0.init();
    indicator0.blink();
    ::IotNode::Utils::UDP::instance.addService(&service0);

    indicator1.init();
    indicator1.blinkRGBInclusive();
    ::IotNode::Utils::UDP::instance.addService(&service1);

    buzzer.init();
    buzzer.melody({OUTPUT_BUZZER_DEFAULT_FREQUENCY,
                   OUTPUT_BUZZER_DEFAULT_FREQUENCY * 2},
                  1, 200, 0);
    ::IotNode::Utils::UDP::instance.addService(&service3);

    ws2812Bus0.begin();
    indicatorRGB.init();
    indicatorRGB.blinkRGBInclusive();
    ::IotNode::Utils::UDP::instance.addService(&service2);

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
