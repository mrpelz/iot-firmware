#include "./setup.h"

#ifdef IOT_NODE_EQ3

namespace IotNode::Utils::EQ3
{
#ifdef IOT_NODE_EQ3_ENTRY_DOOR
  Lib::EQ3::Class entryDoorLock(EQ3_ENTRY_DOOR_ADDRESS, EQ3_ENTRY_DOOR_USER_KEY, EQ3_ENTRY_DOOR_USER_ID);
#endif

  void update()
  {
  }

#ifdef IOT_NODE_ESP32
  void task(void *parameter)
  {
    for (;;)
    {
      update();
      vTaskDelay(IOT_NODE_MUTLITASKING_DELAY / portTICK_PERIOD_MS);
    }
  }
#endif

  void setup()
  {
    BLEDevice::init("");
    entryDoorLock.connect();

#ifdef IOT_NODE_ESP32
    xTaskCreatePinnedToCore(
        task,
        "eq3_maintenance",
        FREERTOS_STACK_SIZE,
        NULL,
        tskIDLE_PRIORITY,
        NULL,
        CONFIG_ARDUINO_RUNNING_CORE);
#endif
  }
}

#endif
