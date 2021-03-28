#ifdef IOT_NODE_CCS811

#include "./main.h"

namespace IotNode {
namespace Services {

namespace Ccs811 {
  Utils::UDP::RespondCallback respondCallback = NULL;

  bool working = false;
  auto sensor = Sensor();

  void initializer(TwoWire *i2c) {
    Utils::Log::debug("ccs811-service", "initializing sensor");

    working = sensor.begin(i2c);
    if (!working) {
      Utils::Log::debug("ccs811-service", "sensor initialization failed");
    }
  }

  void responseTask(void *parameter) {
    if (respondCallback == NULL) {
      vTaskDelete(NULL);
      return;
    }

    auto request = (Utils::UDP::Payload *)parameter;

    if (request->size() < (sizeof(float) * 2)) {
      Utils::Log::debug("ccs811-service", "request does not contain temperature and humidity");

      respondCallback({});
      respondCallback = NULL;

      vTaskDelete(NULL);
      return;
    }
    auto calibrationTemperature = ((float *)request->data())[0];
    auto calibrationHumidity = ((float *)request->data())[1];

    Utils::Log::debug("ccs811-service.calibration-temperature", String(calibrationTemperature));
    Utils::Log::debug("ccs811-service.calibration-humidity", String(calibrationHumidity));

    Utils::I2C::claim();

    sensor.setEnvironmentalData(calibrationHumidity, calibrationTemperature);

    if (sensor.available() && !sensor.readData()) {
      Utils::Log::debug("ccs811-service", "measurement not successful, sending null response");

      Utils::I2C::unclaim();

      respondCallback({});
      respondCallback = NULL;

      vTaskDelete(NULL);
      return;
    }

    auto temperatureReading = sensor.calculateTemperature();
    auto tvocReading = sensor.getTVOC();
    auto eco2Reading = sensor.geteCO2();

    Utils::I2C::unclaim();

    Utils::Log::debug("ccs811-service.temperature", String(temperatureReading));
    Utils::Log::debug("ccs811-service.tvoc", String(tvocReading));
    Utils::Log::debug("ccs811-service.eco2", String(eco2Reading));

    auto temperatureResult = reinterpret_cast<uint8_t*>(&temperatureReading);
    auto tvocResult = reinterpret_cast<uint8_t*>(&tvocReading);
    auto eco2Result = reinterpret_cast<uint8_t*>(&eco2Reading);

    Utils::UDP::Payload response;

    response.insert(
      response.end(),
      temperatureResult,
      temperatureResult + sizeof(temperatureReading)
    );

    response.insert(
      response.end(),
      tvocResult,
      tvocResult + sizeof(tvocReading)
    );

    response.insert(
      response.end(),
      eco2Result,
      eco2Result + sizeof(eco2Reading)
    );

    Utils::Log::debug("ccs811-service", "sending response");

    respondCallback(response);
    respondCallback = NULL;

    vTaskDelete(NULL);
  }

  void handler(Utils::UDP::Payload *request, Utils::UDP::RespondCallback respond) {
    Utils::Log::debug("ccs811-service", "got request");

    if (respondCallback != NULL) return;

    if (!working) {
      Utils::Log::debug("ccs811-service", "sensor not working, sending null response");

      respond({});
      return;
    }

    respondCallback = respond;
    xTaskCreatePinnedToCore(
      responseTask,
      "ccs811_handling",
      2048,
      (void *)request,
      1,
      NULL,
      CONFIG_ARDUINO_RUNNING_CORE
    );
  }
}

} // section namespace
} // project namespace

#endif
