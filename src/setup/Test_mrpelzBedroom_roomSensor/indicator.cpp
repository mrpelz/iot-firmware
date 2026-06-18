#ifdef IOT_NODE_BOARD_ROOM_SENSOR

#include "./indicator.h"

namespace IotNode::Setup::Indicator
{
  ::IotNode::Utils::Indicator::ClassExpander indicator0({ROOM_SENSOR_INDICATOR_LED_R,
                                                         true,
                                                         INDICATOR_BLINK_PERIOD_ON,
                                                         INDICATOR_BLINK_PERIOD_OFF});
  ::IotNode::Utils::Indicator::ClassExpander indicator1({ROOM_SENSOR_INDICATOR_LED_G, true});
  ::IotNode::Utils::Indicator::ClassExpander indicator2({ROOM_SENSOR_INDICATOR_LED_B, true});

  ::IotNode::Utils::Indicator::ClassPin indicatorNotInUse0({5,
                                                            true,
                                                            INDICATOR_BLINK_PERIOD_ON,
                                                            INDICATOR_BLINK_PERIOD_OFF});
  ::IotNode::Utils::Indicator::ClassPin indicatorNotInUse1({17,
                                                            true,
                                                            INDICATOR_BLINK_PERIOD_ON,
                                                            INDICATOR_BLINK_PERIOD_OFF});

  auto service0 = ::IotNode::Services::Indicator::makeService(&indicator0, 0);
  auto service1 = ::IotNode::Services::Indicator::makeService(&indicator1, 1);
  auto service2 = ::IotNode::Services::Indicator::makeService(&indicator2, 2);

  void update()
  {
    indicator0.update();
    indicator1.update();
    indicator2.update();
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
    indicator1.init();
    indicator2.init();
    indicatorNotInUse0.init();
    indicatorNotInUse1.init();

    ::IotNode::Utils::UDP::instance.addService(&service0);
    ::IotNode::Utils::UDP::instance.addService(&service1);
    ::IotNode::Utils::UDP::instance.addService(&service2);

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
