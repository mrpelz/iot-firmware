#ifdef IOT_NODE_TSL2561

#include "./main.h"

namespace IotNode {

namespace Tsl2561 {
  UDP::RespondCallback respondCallback = NULL;

  bool working = false;
  auto sensor = Sensor();
  uint32_t ms;

  void initializer(TwoWire *i2c) {
    Log::debug("tsl2561-service", "initializing sensor");

    uint8_t id;

    sensor.begin(i2c);
    working = sensor.getID(id);

    if (!working) {
      Log::debug("tsl2561-service", "sensor initialization failed");
      return;
    }

    sensor.setTiming(false, 2, ms);
  }

  void responseTask(void * parameter) {
    if (respondCallback == NULL) {
      vTaskDelete(NULL);
      return;
    }

    UDP::Payload response;

    sensor.setPowerUp();

    const TickType_t delay = ms / portTICK_PERIOD_MS;
    vTaskDelay(delay);

    uint32_t data0, data1;
    double lux;

    sensor.getData(data0, data1);
    bool good = sensor.getLux(2, ms, data0, data1, lux);

    sensor.setPowerDown();

    if (!good) {
      respondCallback({});
      return;
    }

    Log::debug("tsl2561-service.lux", String(lux));

    auto result = reinterpret_cast<uint8_t*>(&lux);

    response.insert(response.end(), result, result + sizeof(lux));

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
