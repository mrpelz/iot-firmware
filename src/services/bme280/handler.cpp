#include "./handler.h"

RequestHandler makeBme280Handler() {
  bool working = false;
  Adafruit_BME280 sensor;

  auto initializer = [&]() {
    if (working) return;

    working = sensor.begin();
    if (!working) return;

    sensor.setSampling(
      Adafruit_BME280::MODE_FORCED,
      Adafruit_BME280::SAMPLING_X1, // temperature
      Adafruit_BME280::SAMPLING_X1, // pressure
      Adafruit_BME280::SAMPLING_X1, // humidity
      Adafruit_BME280::FILTER_OFF
    );
  };

  initializer();

  auto handler = [&](
    std::vector<uint8_t> *request,
    std::function<void (std::vector<uint8_t> response)> respond
  ) {
    initializer();

    if (!working) {
      respond({});
      return;
    }

    std::vector<uint8_t> response;

    sensor.takeForcedMeasurement();
    float temperature = sensor.readTemperature();
    float humidity = sensor.readHumidity();
    float pressure = sensor.readPressure();

    response.insert(response.end(), &temperature, &temperature + sizeof(temperature));
    response.insert(response.end(), &humidity, &humidity + sizeof(humidity));
    response.insert(response.end(), &pressure, &pressure + sizeof(pressure));

    respond(response);
  };

  return handler;
}
