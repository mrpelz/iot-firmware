#ifdef IOT_NODE_BME280

#include "./main.h"

namespace IotNode {
namespace Services {

namespace Bme280 {
  Utils::UDP::RespondCallback respondCallback = NULL;
  TaskHandle_t taskHandle = NULL;

  bool working = false;
  auto sensor = Adafruit_BME280();

  void initializer(TwoWire *i2c) {
    Utils::Log::debug("bme280-service", "initializing sensor");

    working = sensor.begin(BME280_ADDRESS_ALTERNATE, i2c);
    if (!working) {
      Utils::Log::debug("bme280-service", "sensor initialization failed");
      return;
    }

    sensor.setSampling(
      Adafruit_BME280::MODE_FORCED,
      Adafruit_BME280::SAMPLING_X1, // temperature
      Adafruit_BME280::SAMPLING_X1, // pressure
      Adafruit_BME280::SAMPLING_X1, // humidity
      Adafruit_BME280::FILTER_OFF
    );

    xTaskCreatePinnedToCore(
      responseTask,
      "bme280_handling",
      2048,
      NULL,
      1,
      &taskHandle,
      CONFIG_ARDUINO_RUNNING_CORE
    );
  }

  void responseTask(void *parameter) {
    for(;;) {
      vTaskDelay(IOT_NODE_MUTLITASKING_DELAY / portTICK_PERIOD_MS);
      if (respondCallback == NULL) {
        vTaskSuspend(NULL);
        continue;
      }

      Utils::I2C::claim();

      auto measurementSuccess = sensor.takeForcedMeasurement();
      if (!measurementSuccess) {
        Utils::Log::debug("bme280-service", "measurement not successful, sending null response");

        Utils::I2C::unclaim();

        respondCallback({});
        respondCallback = NULL;
        continue;
      }

      auto temperatureReading = sensor.readTemperature();
      auto humidityReading = sensor.readHumidity();
      auto pressureReading = sensor.readPressure();

      Utils::I2C::unclaim();

      Utils::Log::debug("bme280-service.temperature", String(temperatureReading));
      Utils::Log::debug("bme280-service.humidity", String(humidityReading));
      Utils::Log::debug("bme280-service.pressure", String(pressureReading));

      auto temperatureResult = reinterpret_cast<uint8_t*>(&temperatureReading);
      auto humidityResult = reinterpret_cast<uint8_t*>(&humidityReading);
      auto pressureResult = reinterpret_cast<uint8_t*>(&pressureReading);

      Utils::UDP::Payload response;

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

      Utils::Log::debug("bme280-service", "sending response");

      respondCallback(response);      
      respondCallback = NULL;
    }
  }

  void handler(Utils::UDP::Payload *request, Utils::UDP::RespondCallback respond) {
    Utils::Log::debug("bme280-service", "got request");

    if (!working) {
      Utils::Log::debug("bme280-service", "sensor not working, sending null response");

      respond({});
      return;
    }

    respondCallback = respond;
    if (taskHandle != NULL) vTaskResume(taskHandle);
  }
}

} // section namespace
} // project namespace

#endif
