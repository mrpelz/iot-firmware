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
    #ifdef IOT_NODE_LOGGING
      Utils::Log::debug("bme280-service", "initializing sensor");
    #endif

    working = sensor.begin(BME280_ADDRESS_ALTERNATE, i2c);
    if (!working) {
      #ifdef IOT_NODE_LOGGING
        Utils::Log::debug("bme280-service", "sensor initialization failed");
      #endif

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
      if (respondCallback == NULL) {
        vTaskSuspend(NULL);
        continue;
      }

      Utils::I2C::claim();

      auto measurementSuccess = sensor.takeForcedMeasurement();
      if (!measurementSuccess) {
        #ifdef IOT_NODE_LOGGING
          Utils::Log::debug("bme280-service", "measurement not successful, sending null response");
        #endif

        Utils::I2C::unclaim();

        respondCallback({});
        respondCallback = NULL;
        continue;
      }

      auto temperatureReading = sensor.readTemperature();
      auto humidityReading = sensor.readHumidity();
      auto pressureReading = sensor.readPressure();

      Utils::I2C::unclaim();

      #ifdef IOT_NODE_LOGGING
        Utils::Log::debug("bme280-service.temperature", String(temperatureReading));
        Utils::Log::debug("bme280-service.humidity", String(humidityReading));
        Utils::Log::debug("bme280-service.pressure", String(pressureReading));
      #endif

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

      #ifdef IOT_NODE_LOGGING
        Utils::Log::debug("bme280-service", "sending response");
      #endif

      respondCallback(response);      
      respondCallback = NULL;
    }
  }

  void handler(Utils::UDP::Payload *request, Utils::UDP::RespondCallback respond, Utils::UDP::Peer peer) {
    #ifdef IOT_NODE_LOGGING
      Utils::Log::debug("bme280-service", "got request");
    #endif

    if (!working) {
      #ifdef IOT_NODE_LOGGING
        Utils::Log::debug("bme280-service", "sensor not working, sending null response");
      #endif

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
