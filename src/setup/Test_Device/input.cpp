#ifdef IOT_NODE_BOARD_TEST_DEVICE

#include "./input.h"

namespace IotNode::Setup::Input
{
  ::IotNode::Events::Input::Class input0(39, true, 50, 0);

  void update()
  {
    input0.update();
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
    auto event0 = ::IotNode::Events::Input::makeEvent(&::IotNode::Utils::UDP::instance, 0);
    input0.setChangeCallback(event0);
    input0.start();

    xTaskCreatePinnedToCore(
        task,
        "input_maintenance",
        FREERTOS_STACK_SIZE,
        NULL,
        tskIDLE_PRIORITY,
        NULL,
        CONFIG_ARDUINO_RUNNING_CORE);
  }
}

#endif
