#ifdef ARDUINO_ARCH_ESP32

#include "./main.h"

namespace IotNode {
namespace Services {

namespace Async {
  Utils::UDP::RespondCallback respondCallback = NULL;

  void responseTask(void *parameter) {
    if (respondCallback == NULL) {
      vTaskDelete(NULL);
      return;
    }

    vTaskDelay(ASYNC_RESPONSE_DELAY / portTICK_PERIOD_MS);

    Utils::Log::debug("async-service", "sending delayed response");

    respondCallback({ 0x0a, 0x0b, 0x0c });
    respondCallback = NULL;

    vTaskDelete(NULL);
  }

  void handler(Utils::UDP::Payload *request, Utils::UDP::RespondCallback respond) {
    Utils::Log::debug("async-service", "got request");

    if (respondCallback != NULL) return;

    respondCallback = respond;
    xTaskCreatePinnedToCore(
      responseTask,
      "async_handling",
      2048,
      NULL,
      1,
      NULL,
      CONFIG_ARDUINO_RUNNING_CORE
    );
  }
}

} // section namespace
} // project namespace

#endif
