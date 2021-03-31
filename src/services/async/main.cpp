#ifdef ARDUINO_ARCH_ESP32

#include "./main.h"

namespace IotNode {
namespace Services {

namespace Async {
  Utils::UDP::RespondCallback respondCallback = NULL;
  TaskHandle_t taskHandle = NULL;

  void initializer() {
    xTaskCreatePinnedToCore(
      responseTask,
      "async_handling",
      2048,
      NULL,
      1,
      &taskHandle,
      CONFIG_ARDUINO_RUNNING_CORE
    );
  }

  void responseTask(void *parameter) {
    for(;;) {
      vTaskDelay(IOT_NODE_MUTLITASKING_DELAY / portTICK_PERIOD_MS);
      if (respondCallback == NULL) {
        vTaskSuspend(NULL);
        continue;
      }

      vTaskDelay(ASYNC_RESPONSE_DELAY / portTICK_PERIOD_MS);

      Utils::Log::debug("async-service", "sending delayed response");

      respondCallback({ 0x0a, 0x0b, 0x0c });
      respondCallback = NULL;
    }
  }

  void handler(Utils::UDP::Payload *request, Utils::UDP::RespondCallback respond) {
    Utils::Log::debug("async-service", "got request");

    respondCallback = respond;
    if (taskHandle != NULL) vTaskResume(taskHandle);
  }
}

} // section namespace
} // project namespace

#endif
