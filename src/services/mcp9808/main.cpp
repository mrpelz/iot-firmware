#ifdef IOT_NODE_MCP9808

#include "./main.h"

namespace IotNode {
namespace Services {

namespace Mcp9808 {
  Utils::UDP::RespondCallback respondCallback = NULL;
  TaskHandle_t taskHandle = NULL;

  bool working = false;
  auto sensor = Adafruit_MCP9808();

  void initializer(TwoWire *i2c) {
    Utils::Log::debug("mcp9808-service", "initializing sensor");

    working = sensor.begin(MCP9808_I2CADDR_DEFAULT, i2c);
    if (!working) {
      Utils::Log::debug("mcp9808-service", "sensor initialization failed");
      return;
    }

    sensor.setResolution(3);
    sensor.shutdown();
  }

  void responseTask(void *parameter) {
    Utils::I2C::claim();

    sensor.wake();

    auto reading = sensor.readTempC();

    sensor.shutdown();

    Utils::I2C::unclaim();

    Utils::Log::debug("mcp9808-service.reading", String(reading));

    auto result = reinterpret_cast<uint8_t*>(&reading);

    Utils::UDP::Payload response;

    response.insert(
      response.end(),
      result,
      result + sizeof(reading)
    );

    Utils::Log::debug("mcp9808-service", "sending response");

    respondCallback(response);

    taskHandle = NULL;
    vTaskDelete(NULL);
  }

  void handler(Utils::UDP::Payload *request, Utils::UDP::RespondCallback respond) {
    Utils::Log::debug("mcp9808-service", "got request");

    if (!working) {
      Utils::Log::debug("mcp9808-service", "sensor not working, sending null response");

      respond({});
      return;
    }

    respondCallback = respond;

    if(taskHandle != NULL) {
      return;
    }

    xTaskCreatePinnedToCore(
      responseTask,
      "mcp9808_handling",
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
