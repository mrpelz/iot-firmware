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

    auto measurementSuccess = sensor.takeForcedMeasurement();
    if (!measurementSuccess) {
      Log::debug("bme280-service", "measurement not successful, sending null response");

      respondCallback({});
      respondCallback = NULL;

      vTaskDelete(NULL);
      return;
    }

    UDP::Payload response;

    auto temperatureReading = sensor.readTemperature();
    auto humidityReading = sensor.readHumidity();
    auto pressureReading = sensor.readPressure();

    Log::debug("bme280-service.temperature", String(temperatureReading));
    Log::debug("bme280-service.humidity", String(humidityReading));
    Log::debug("bme280-service.pressure", String(pressureReading));

    auto temperatureResult = reinterpret_cast<uint8_t*>(&temperatureReading);
    auto humidityResult = reinterpret_cast<uint8_t*>(&humidityReading);
    auto pressureResult = reinterpret_cast<uint8_t*>(&pressureReading);

    response.insert(
      response.end(),
      temperatureResult,
      temperatureResult + sizeof(temperatureReading)
    );

    response.insert(
      response.end(),
      humidityResult,
      humidityResult + sizeof(humidityReading)
    );

    response.insert(
      response.end(),
      pressureResult,
      pressureResult + sizeof(pressureReading)
    );

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
