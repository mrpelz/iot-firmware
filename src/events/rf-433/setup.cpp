#include "./setup.h"

#ifdef IOT_NODE_RF_433

namespace IotNode::Events::Rf433
{
  auto rf = RCSwitch();

  void update()
  {
    if (rf.available())
    {
      auto protocol = rf.getReceivedProtocol();
      auto bitLength = rf.getReceivedBitlength();
      auto value = rf.getReceivedValue();

#ifdef IOT_NODE_LOGGING
      Utils::Log::debug(fmt::format("event.rf433.protocol: {}", protocol));
      Utils::Log::debug(fmt::format("event.rf433.delay: {}", rf.getReceivedDelay()));
      Utils::Log::debug(fmt::format("event.rf433.length: {}", bitLength));
      Utils::Log::debug(fmt::format("event.rf433.value: {}", value));
#endif

      onDataReceived(&protocol, &bitLength, &value);

      rf.resetAvailable();
    }
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
    rf.enableReceive(digitalPinToInterrupt(4));

#ifdef IOT_NODE_ESP32
    xTaskCreatePinnedToCore(
        task,
        "rf433_maintenance",
        FREERTOS_STACK_SIZE,
        NULL,
        tskIDLE_PRIORITY,
        NULL,
        CONFIG_ARDUINO_RUNNING_CORE);
#endif
  }
}

#endif
