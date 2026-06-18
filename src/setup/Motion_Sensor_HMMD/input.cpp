#ifdef IOT_NODE_BOARD_MOTION_SENSOR_HMMD

#include "./input.h"

namespace IotNode::Setup::Input
{
  ::IotNode::Events::Input::Class input0(7, true, 0, 0);
  ::IotNode::Events::Input::Class input1(8, true, 0, 0);
  ::IotNode::Events::Input::Class input2(9, true, 0, 0);

  void update()
  {
    input0.update();
    input1.update();
    input2.update();
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
    auto event0 = ::IotNode::Events::Input::makeEvent(&::IotNode::Utils::UDP::instance, 0);
    input0.setChangeCallback(event0);
    input0.start();

    auto event1 = ::IotNode::Events::Input::makeEvent(&::IotNode::Utils::UDP::instance, 1);
    input1.setChangeCallback(event1);
    input1.start();

    auto event2 = ::IotNode::Events::Input::makeEvent(&::IotNode::Utils::UDP::instance, 2);
    input2.setChangeCallback(event2);
    input2.start();

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
