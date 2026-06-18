#ifdef IOT_NODE_ESP32

#include "./main.h"

namespace IotNode::Services::Async
{
  Utils::UDP::RespondCallback respondCallback = NULL;
  TaskHandle_t taskHandle = NULL;

  void initializer()
  {
    xTaskCreatePinnedToCore(
        responseTask,
        "async_handling",
        FREERTOS_STACK_SIZE,
        NULL,
        tskIDLE_PRIORITY,
        &taskHandle,
        CONFIG_ARDUINO_RUNNING_CORE);
  }

  void responseTask(void *parameter)
  {
    for (;;)
    {
      if (respondCallback == NULL)
      {
        vTaskSuspend(NULL);
        continue;
      }

      vTaskDelay(pdMS_TO_TICKS(100));

#ifdef IOT_NODE_LOGGING
      Utils::Log::debug("async-service: sending delayed response");
#endif

      respondCallback({0x0a, 0x0b, 0x0c});
      respondCallback = NULL;
    }
  }

  void handler(Utils::UDP::Payload *request, Utils::UDP::RespondCallback respond, Utils::UDP::Peer peer)
  {
#ifdef IOT_NODE_LOGGING
    Utils::Log::debug("async-service: got request");
#endif

    respondCallback = respond;
    if (taskHandle != NULL)
      vTaskResume(taskHandle);
  }
}

#endif
