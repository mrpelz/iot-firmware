#ifdef IOT_NODE_VEML6070

#include "./main.h"

namespace IotNode {
namespace Services {

namespace Veml6070 {
  Utils::UDP::RespondCallback respondCallback = NULL;

  bool working = false;
  auto sensor = Sensor();

  void initializer(TwoWire *i2c) {
    Utils::Log::debug("veml6070-service", "initializing sensor");

    working = sensor.begin(i2c);
    if (!working) {
      Utils::Log::debug("veml6070-service", "sensor initialization failed");
      return;
    }

    sensor.sleep(true);
  }

  void responseTask(void *parameter) {
    if (respondCallback == NULL) {
      vTaskDelete(NULL);
      return;
    }

    Utils::I2C::claim();

    sensor.sleep(false);
    sensor.waitForNext();

    auto reading = sensor.readUV();

    sensor.sleep(true);

    Utils::I2C::unclaim();

    Utils::Log::debug("veml6070-service.reading", String(reading));

    auto result = reinterpret_cast<uint8_t*>(&reading);

    Utils::UDP::Payload response;

    response.insert(
      response.end(),
      result,
      result + sizeof(reading)
    );

    Utils::Log::debug("veml6070-service", "sending response");

    respondCallback(response);
    respondCallback = NULL;


    vTaskDelete(NULL);
  }

  void handler(Utils::UDP::Payload *request, Utils::UDP::RespondCallback respond) {
    Utils::Log::debug("veml6070-service", "got request");

    if (respondCallback != NULL) return;

    if (!working) {
      Utils::Log::debug("veml6070-service", "sensor not working, sending null response");

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

} // section namespace
} // project namespace

#endif
