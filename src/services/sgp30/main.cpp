#ifdef IOT_NODE_SGP30

#include "./main.h"

namespace IotNode {
namespace Services {

namespace Sgp30 {
  Utils::UDP::RespondCallback respondCallback = NULL;
  TaskHandle_t taskHandle = NULL;

  bool working = false;
  auto sensor = Adafruit_SGP30();

  uint32_t getAbsoluteHumidity(float temperature, float humidity) {
    // approximation formula from Sensirion SGP30 Driver Integration chapter 3.15
    const float absoluteHumidity = 216.7f * ((humidity / 100.0f) * 6.112f * exp((17.62f * temperature) / (243.12f + temperature)) / (273.15f + temperature)); // [g/m^3]
    const uint32_t absoluteHumidityScaled = static_cast<uint32_t>(1000.0f * absoluteHumidity); // [mg/m^3]
    return absoluteHumidityScaled;
  }

  void initializer(TwoWire *i2c) {
    Utils::Log::debug("sgp30-service", "initializing sensor");

    working = sensor.begin(i2c);
    if (!working) {
      Utils::Log::debug("sgp30-service", "sensor initialization failed");
    }
  }

  void responseTask(void *parameter) {
    auto request = (Utils::UDP::Payload *)parameter;

    if (request->size() < (sizeof(float) * 2)) {
      Utils::Log::debug("sgp30-service", "request does not contain temperature and humidity");

      respondCallback({});

      taskHandle = NULL;
      vTaskDelete(NULL);
      return;
    }
    auto calibrationTemperature = ((float *)request->data())[0];
    auto calibrationHumidity = ((float *)request->data())[1];

    Utils::Log::debug("sgp30-service.calibration-temperature", String(calibrationTemperature));
    Utils::Log::debug("sgp30-service.calibration-humidity", String(calibrationHumidity));

    Utils::I2C::claim();

    auto calibrationSuccess = sensor.setHumidity(getAbsoluteHumidity(calibrationTemperature, calibrationHumidity));
    if (!calibrationSuccess) {
      Utils::Log::debug("sgp30-service", "calibration not successful, sending null response");

      Utils::I2C::unclaim();

      respondCallback({});

      taskHandle = NULL;
      vTaskDelete(NULL);
      return;
    }

    auto measurementRawSuccess = sensor.IAQmeasureRaw();
    auto measurementSuccess = sensor.IAQmeasure();

    Utils::I2C::unclaim();

    if (!measurementRawSuccess || !measurementSuccess
    ) {
      Utils::Log::debug("sgp30-service", "measurement not successful, sending null response");

      respondCallback({});

      taskHandle = NULL;
      vTaskDelete(NULL);
      return;
    }

    auto h2Reading = sensor.rawH2;
    auto ethanolReading = sensor.rawEthanol;
    auto tvocReading = sensor.TVOC;
    auto eco2Reading = sensor.eCO2;

    Utils::Log::debug("sgp30-service.h2", String(h2Reading));
    Utils::Log::debug("sgp30-service.ethanol", String(ethanolReading));
    Utils::Log::debug("sgp30-service.tvoc", String(tvocReading));
    Utils::Log::debug("sgp30-service.eco2", String(eco2Reading));

    auto h2Result = reinterpret_cast<uint8_t*>(&h2Reading);
    auto ethanolResult = reinterpret_cast<uint8_t*>(&ethanolReading);
    auto tvocResult = reinterpret_cast<uint8_t*>(&tvocReading);
    auto eco2Result = reinterpret_cast<uint8_t*>(&eco2Reading);

    Utils::UDP::Payload response;

    response.insert(
      response.end(),
      h2Result,
      h2Result + sizeof(h2Reading)
    );

    response.insert(
      response.end(),
      ethanolResult,
      ethanolResult + sizeof(ethanolReading)
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

    Utils::Log::debug("sgp30-service", "sending response");

    respondCallback(response);

    taskHandle = NULL;
    vTaskDelete(NULL);
  }

  void handler(Utils::UDP::Payload *request, Utils::UDP::RespondCallback respond) {
    Utils::Log::debug("sgp30-service", "got request");

    if (!working) {
      Utils::Log::debug("sgp30-service", "sensor not working, sending null response");

      respond({});
      return;
    }

    respondCallback = respond;

    if(taskHandle != NULL) {
      return;
    }

    xTaskCreatePinnedToCore(
      responseTask,
      "sgp30_handling",
      2048,
      NULL,
      1,
      &taskHandle,
      CONFIG_ARDUINO_RUNNING_CORE
    );
  }
}

} // section namespace
} // project namespace

#endif
