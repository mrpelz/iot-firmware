#ifdef ARDUINO_ARCH_ESP32

#include "./main.h"

namespace IotNode {
namespace Services {

namespace Async {
  Utils::UDP::RespondCallback respondCallback = NULL;
  TaskHandle_t taskHandle = NULL;

  void responseTask(void *parameter) {
    vTaskDelay(ASYNC_RESPONSE_DELAY / portTICK_PERIOD_MS);

    Utils::Log::debug("async-service", "sending delayed response");

    respondCallback({ 0x0a, 0x0b, 0x0c });

    taskHandle = NULL;
    vTaskDelete(NULL);
  }

  void handler(Utils::UDP::Payload *request, Utils::UDP::RespondCallback respond) {
    Utils::Log::debug("async-service", "got request");

    respondCallback = respond;

    if(taskHandle != NULL) {
      return;
    }

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
}

} // section namespace
} // project namespace

#endif
