#ifdef IOT_NODE_TSL2561

#include "./main.h"

namespace IotNode {

namespace Tsl2561 {
  UDP::RespondCallback respondCallback = NULL;

  bool working = false;
  auto sensor = Sensor();

  void initializer(TwoWire *i2c) {
    Log::debug("tsl2561-service", "initializing sensor");

    working = sensor.begin(i2c);

    if (!working) {
      Log::debug("tsl2561-service", "sensor initialization failed");
      return;
    }

    sensor.enableAutoRange(true);
    sensor.setIntegrationTime(TSL2561_INTEGRATIONTIME_402MS);
  }

  void responseTask(void *parameter) {
    if (respondCallback == NULL) {
      vTaskDelete(NULL);
      return;
    }

    uint16_t broadband;
    uint16_t infrared;

    sensor.getLuminosity(&broadband, &infrared);
    auto reading = sensor.calculateLux(broadband, infrared);

    Log::debug("tsl2561-service.reading", String(reading));

    auto result = reinterpret_cast<uint8_t*>(&reading);

    UDP::Payload response;

    response.insert(response.end(), result, result + sizeof(reading));

    Log::debug("tsl2561-service", "sending response");

    respondCallback(response);
    respondCallback == NULL;

    vTaskDelete(NULL);
  }

  void handler(UDP::Payload *request, UDP::RespondCallback respond) {
    Log::debug("tsl2561-service", "got request");

    if (!working) {
      Log::debug("tsl2561-service", "sensor not working, sending null response");

      respond({});
      return;
    }

    respondCallback = respond;
    xTaskCreatePinnedToCore(
      responseTask,
      "tsl2561_handling",
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
