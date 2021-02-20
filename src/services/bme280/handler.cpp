#include "./handler.h"

namespace IotNode {

RequestHandler makeBme280Handler() {
  auto working = std::make_shared<bool>(false);
  auto sensor = std::make_shared<Adafruit_BME280>(Adafruit_BME280());

  auto initializer = [&, working, sensor]() {
    if (*working) return;

    Log::debug("bme280-service", "initializing sensor");

    *working = sensor->begin();
    if (!working) {
      Log::debug("bme280-service", "sensor initialization failed");
      return;
    }

    sensor->setSampling(
      Adafruit_BME280::MODE_FORCED,
      Adafruit_BME280::SAMPLING_X1, // temperature
      Adafruit_BME280::SAMPLING_X1, // pressure
      Adafruit_BME280::SAMPLING_X1, // humidity
      Adafruit_BME280::FILTER_OFF
    );
  };

  auto handler = [&, working, sensor](
    std::vector<uint8_t> *request,
    std::function<void (std::vector<uint8_t> response)> respond
  ) {
    Log::debug("bme280-service", "got request");

    initializer();

    if (!*working) {
      Log::debug("bme280-service", "sensor not working, sending null response");

      respond({});
      return;
    }

    std::vector<uint8_t> response;

    sensor->takeForcedMeasurement();
    float temperature = sensor->readTemperature();
    float humidity = sensor->readHumidity();
    float pressure = sensor->readPressure();

    response.insert(response.end(), &temperature, &temperature + sizeof(temperature));
    response.insert(response.end(), &humidity, &humidity + sizeof(humidity));
    response.insert(response.end(), &pressure, &pressure + sizeof(pressure));

    Log::debug("bme280-service", "sending response");

    respond(response);
  };

  return handler;
}

} // project namespace
