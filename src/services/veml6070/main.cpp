#ifdef IOT_NODE_VEML6070

#include "./main.h"

namespace IotNode {

namespace Veml6070 {
  UDP::RespondCallback respondCallback = NULL;

  bool working = false;
  auto sensor = Sensor();

  void initializer(TwoWire *i2c) {
    Log::debug("veml6070-service", "initializing sensor");

    working = sensor.begin(i2c);
    if (!working) {
      Log::debug("veml6070-service", "sensor initialization failed");
      return;
    }

    sensor.sleep(true);
  }

  void responseTask(void *parameter) {
    if (respondCallback == NULL) {
      vTaskDelete(NULL);
      return;
    }

    sensor.sleep(false);
    sensor.waitForNext();

    auto reading = sensor.readUV();

    sensor.sleep(true);

    Log::debug("veml6070-service.reading", String(reading));

    auto result = reinterpret_cast<uint8_t*>(&reading);

    UDP::Payload response;

    response.insert(
      response.end(),
      result,
      result + sizeof(reading)
    );

    Log::debug("veml6070-service", "sending response");

    respondCallback(response);
    respondCallback == NULL;


    vTaskDelete(NULL);
  }

  void handler(UDP::Payload *request, UDP::RespondCallback respond) {
    Log::debug("veml6070-service", "got request");

    if (!working) {
      Log::debug("veml6070-service", "sensor not working, sending null response");

      respond({});
      return;
    }

    respondCallback = respond;
    xTaskCreatePinnedToCore(
      responseTask,
      "veml6070_handling",
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
