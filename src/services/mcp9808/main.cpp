#ifdef IOT_NODE_MCP9808

#include "./main.h"

namespace IotNode {

namespace Mcp9808 {
  UDP::RespondCallback respondCallback = NULL;

  bool working = false;
  auto sensor = Adafruit_MCP9808();

  void initializer() {
    Log::debug("mcp9808-service", "initializing sensor");

    working = sensor.begin();
    if (!working) {
      Log::debug("mcp9808-service", "sensor initialization failed");
      return;
    }

    sensor.setResolution(3);
  }

  void responseTask(void * parameter) {
    if (respondCallback == NULL) {
      vTaskDelete(NULL);
      return;
    }

    UDP::Payload response;

    sensor.wake();
    float reading = sensor.readTempC();

    response.insert(response.end(), &reading, &reading + sizeof(reading));

    Log::debug("mcp9808-service", "sending response");

    respondCallback(response);
    respondCallback == NULL;

    const TickType_t delay = 2000 / portTICK_PERIOD_MS;
    vTaskDelay(delay);

    sensor.shutdown();

    vTaskDelete(NULL);
  }

  void handler(UDP::Payload *request, UDP::RespondCallback respond) {
    Log::debug("mcp9808-service", "got request");

    if (!working) {
      Log::debug("mcp9808-service", "sensor not working, sending null response");

      respond({});
      return;
    }

    respondCallback = respond;
    xTaskCreatePinnedToCore(
      responseTask,
      "mcp9808_handling",
      2048,
      NULL,
      1,
      NULL,
      CONFIG_ARDUINO_RUNNING_CORE
    );
  }
}

} // project namespace

#endif
