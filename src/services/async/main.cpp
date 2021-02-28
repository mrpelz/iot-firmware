#include "./main.h"

namespace IotNode {

namespace Async {
  UDP::RespondCallback respondCallback = NULL;

  void responseTask(void * parameter) {
    if (respondCallback == NULL) {
      vTaskDelete(NULL);
      return;
    }

    const TickType_t xDelay = ASYNC_RESPONSE_DELAY / portTICK_PERIOD_MS;
    vTaskDelay(xDelay);

    Log::debug("async-service", "sending delayed response");

    respondCallback({ 0x0a, 0x0b, 0x0c });
    respondCallback == NULL;

    vTaskDelete(NULL);
  }

  void handler(UDP::Payload *request, UDP::RespondCallback respond) {
    Log::debug("async-service", "got request");

    respondCallback = respond;
    xTaskCreate(responseTask, "async_handling", 10000, NULL, 1, NULL);
  }
}

} // project namespace
