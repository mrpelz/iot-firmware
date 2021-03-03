#ifdef IOT_NODE_BME280

#include "./main.h"

namespace IotNode {

namespace Bme280 {
  UDP::RespondCallback respondCallback = NULL;

  bool working = false;
  auto sensor = Adafruit_BME280();

  void initializer(TwoWire *i2c) {
    Log::debug("bme280-service", "initializing sensor");

    working = sensor.begin(BME280_ADDRESS_ALTERNATE, i2c);
    if (!working) {
      Log::debug("bme280-service", "sensor initialization failed");
      return;
    }

    sensor.setSampling(
      Adafruit_BME280::MODE_FORCED,
      Adafruit_BME280::SAMPLING_X1, // temperature
      Adafruit_BME280::SAMPLING_X1, // pressure
      Adafruit_BME280::SAMPLING_X1, // humidity
      Adafruit_BME280::FILTER_OFF
    );
  }

  void responseTask(void * parameter) {
    if (respondCallback == NULL) {
      vTaskDelete(NULL);
      return;
    }

    UDP::Payload response;

    sensor.takeForcedMeasurement();
    float temperature = sensor.readTemperature();
    float humidity = sensor.readHumidity();
    float pressure = sensor.readPressure();

    Log::debug("bme280-service.temperature", String(temperature));
    Log::debug("bme280-service.humidity", String(humidity));
    Log::debug("bme280-service.pressure", String(pressure));

    response.insert(response.end(), &temperature, &temperature + sizeof(temperature));
    response.insert(response.end(), &humidity, &humidity + sizeof(humidity));
    response.insert(response.end(), &pressure, &pressure + sizeof(pressure));

    Log::debug("bme280-service", "sending response");

    respondCallback(response);
    respondCallback = NULL;

    vTaskDelete(NULL);
  }

  void handler(UDP::Payload *request, UDP::RespondCallback respond) {
    Log::debug("bme280-service", "got request");

    if (!working) {
      Log::debug("bme280-service", "sensor not working, sending null response");

      respond({});
      return;
    }

    respondCallback = respond;
    xTaskCreatePinnedToCore(
      responseTask,
      "bme280_handling",
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
