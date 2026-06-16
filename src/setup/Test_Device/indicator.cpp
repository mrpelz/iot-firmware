#ifdef IOT_NODE_BOARD_TEST_DEVICE

#include "./indicator.h"

namespace IotNode::Setup::Indicator
{
  ::IotNode::Utils::Indicator::ClassPin indicator0({17,
                                                    true,
                                                    INDICATOR_BLINK_PERIOD_ON,
                                                    INDICATOR_BLINK_PERIOD_OFF});

  auto service0 = ::IotNode::Services::Indicator::makeService(&indicator0, 0);

  void update()
  {
    indicator0.update();
  }

  void task(void *parameter)
  {
    for (;;)
    {
      update();
      vTaskDelay(IOT_NODE_MUTLITASKING_DELAY / portTICK_PERIOD_MS);
    }
  }

  void setup()
  {
    indicator0.init();
    indicator0.setOn(true);

    ::IotNode::Utils::UDP::instance.addService(&service0);

    xTaskCreatePinnedToCore(
        task,
        "indicator_maintenance",
        FREERTOS_STACK_SIZE,
        NULL,
        tskIDLE_PRIORITY,
        NULL,
        CONFIG_ARDUINO_RUNNING_CORE);
  }
}

#endif
