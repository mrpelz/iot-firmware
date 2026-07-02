#ifdef IOT_NODE_BOARD_WAVESHARE_ESP32_S3_ZERO

#include "./output-ng.h"

namespace IotNode::Setup::OutputNg
{
  ESP32_WS2812 ws2812Bus0 = ESP32_WS2812(1, 21, 0, TYPE_RGB);
  ::IotNode::Utils::OutputNg::DimmableRGBWS2812 indicatorRGB0(0, &ws2812Bus0, false);
  auto indicatorRGB0Service = ::IotNode::Services::OutputNg::makeService(&indicatorRGB0, &::IotNode::Utils::UDP::instance, 0);

  ::IotNode::Utils::OutputNg::Dimmable led0(1, true);
  auto led0Service = ::IotNode::Services::OutputNg::makeService(&led0, &::IotNode::Utils::UDP::instance, 0);

  ::IotNode::Utils::OutputNg::DimmableRGB ledRGB1(2, 3, 4, false);
  auto ledRGB1Service = ::IotNode::Services::OutputNg::makeService(&ledRGB1, &::IotNode::Utils::UDP::instance, 1);

  ::IotNode::Utils::OutputNg::Buzzer buzzer0(5, true);
  auto buzzer0Service = ::IotNode::Services::OutputNg::makeService(&buzzer0, &::IotNode::Utils::UDP::instance, 0);

  void update()
  {
    indicatorRGB0.update();
    ws2812Bus0.show();

    led0.update();

    ledRGB1.update();

    buzzer0.update();
  }

  void task(void *parameter)
  {
    for (;;)
    {
      update();
      vTaskDelay(pdMS_TO_TICKS(10));
    }
  }

  void setup()
  {
    ws2812Bus0.begin();
    indicatorRGB0.init();
    indicatorRGB0.blinkRGB();
    ::IotNode::Utils::UDP::instance.addService(&indicatorRGB0Service);

    led0.init();
    led0.blink();
    ::IotNode::Utils::UDP::instance.addService(&led0Service);

    ledRGB1.init();
    ledRGB1.blinkRGBInclusive();
    ::IotNode::Utils::UDP::instance.addService(&ledRGB1Service);

    buzzer0.init();
    buzzer0.melody({OUTPUT_BUZZER_DEFAULT_FREQUENCY,
                    OUTPUT_BUZZER_DEFAULT_FREQUENCY * 2},
                   1, 200, 0);
    ::IotNode::Utils::UDP::instance.addService(&buzzer0Service);

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
