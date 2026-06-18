#include "./setup.h"

namespace IotNode::Utils::Keepalive
{
  Class keepalive(
      3600000, // one hour
      []()
      {
        ESP.restart();
      });

  Class eventPeer(
      10000, // 10 seconds
      []()
      {
        Utils::UDP::instance.removeEventPeer();
      });

#ifdef IOT_NODE_POWER_CYCLE_PROTECTION
  Class powerCycleProtection(
      30000,
      []()
      {
#ifdef ENV_SCOPED_SETUP
        Setup::Output::output0.setOn(true);
#else
        Output::output0.setOn(true);
#endif
      });
#endif

  void update()
  {
    keepalive.update();
    eventPeer.update();

#ifdef IOT_NODE_POWER_CYCLE_PROTECTION
    powerCycleProtection.update();
#endif
  }

#ifdef IOT_NODE_ESP32
  void task(void *parameter)
  {
    for (;;)
    {
      update();
      vTaskDelay(pdMS_TO_TICKS(100));
    }
  }
#endif

  void setup()
  {
#ifdef IOT_NODE_ESP32
    xTaskCreatePinnedToCore(
        task,
        "keepalive_maintenance",
        FREERTOS_STACK_SIZE,
        NULL,
        tskIDLE_PRIORITY,
        NULL,
        CONFIG_ARDUINO_RUNNING_CORE);
#endif

#ifdef IOT_NODE_POWER_CYCLE_PROTECTION
    powerCycleProtection.tick();
#endif
  }
}
