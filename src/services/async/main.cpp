#include "./main.h"

namespace IotNode {

namespace Async {
  UDP::RespondCallback respondCallback = NULL;
  const TickType_t delay = ASYNC_RESPONSE_DELAY / portTICK_PERIOD_MS;

  void responseTask(void * parameter) {
    if (respondCallback == NULL) {
      vTaskDelete(NULL);
      return;
    }

    vTaskDelay(delay);

    Log::debug("async-service", "sending delayed response");

    respondCallback({ 0x0a, 0x0b, 0x0c });
    respondCallback == NULL;

    vTaskDelete(NULL);
  }

  void handler(UDP::Payload *request, UDP::RespondCallback respond) {
    Log::debug("async-service", "got request");

    respondCallback = respond;
    xTaskCreate(responseTask, "async_handling", 2048, NULL, 1, NULL);
  }
}

} // project namespace
