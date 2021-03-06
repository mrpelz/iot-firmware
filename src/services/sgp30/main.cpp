#ifdef IOT_NODE_SGP30

#include "./main.h"

namespace IotNode {

namespace Sgp30 {
  UDP::RespondCallback respondCallback = NULL;

  bool working = false;
  auto sensor = Adafruit_SGP30();

  uint32_t getAbsoluteHumidity(float temperature, float humidity) {
    // approximation formula from Sensirion SGP30 Driver Integration chapter 3.15
    const float absoluteHumidity = 216.7f * ((humidity / 100.0f) * 6.112f * exp((17.62f * temperature) / (243.12f + temperature)) / (273.15f + temperature)); // [g/m^3]
    const uint32_t absoluteHumidityScaled = static_cast<uint32_t>(1000.0f * absoluteHumidity); // [mg/m^3]
    return absoluteHumidityScaled;
  }

  void initializer(TwoWire *i2c) {
    Log::debug("sgp30-service", "initializing sensor");

    working = sensor.begin(i2c);
    if (!working) {
      Log::debug("sgp30-service", "sensor initialization failed");
    }
  }

  void responseTask(void *parameter) {
    if (respondCallback == NULL) {
      vTaskDelete(NULL);
      return;
    }

    auto request = (UDP::Payload *)parameter;

    if (request->size() < (sizeof(float) * 2)) {
      Log::debug("sgp30-service", "request does not contain temperature and humidity");

      respondCallback({});
      respondCallback = NULL;

      vTaskDelete(NULL);
      return;
    }
    auto calibrationTemperature = ((float *)request->data())[0];
    auto calibrationHumidity = ((float *)request->data())[1];

    Log::debug("sgp30-service.calibration-temperature", String(calibrationTemperature));
    Log::debug("sgp30-service.calibration-humidity", String(calibrationHumidity));

    auto calibrationSuccess = sensor.setHumidity(getAbsoluteHumidity(calibrationTemperature, calibrationHumidity));
    if (!calibrationSuccess) {
      Log::debug("sgp30-service", "calibration not successful, sending null response");

      respondCallback({});
      respondCallback = NULL;

      vTaskDelete(NULL);
      return;
    }

    uint16_t eco2BaseReading;
    uint16_t tvocBaseReading;

    auto measurementBaseSuccess = sensor.getIAQBaseline(
      &eco2BaseReading,
      &tvocBaseReading
    );
    auto measurementRawSuccess = sensor.IAQmeasureRaw();
    auto measurementSuccess = sensor.IAQmeasure();
    if (
      !measurementBaseSuccess
      || !measurementRawSuccess
      || !measurementSuccess
    ) {
      Log::debug("sgp30-service", "measurement not successful, sending null response");

      respondCallback({});
      respondCallback = NULL;

      vTaskDelete(NULL);
      return;
    }

    UDP::Payload response;

    auto h2Reading = sensor.rawH2;
    auto ethanolReading = sensor.rawEthanol;
    auto tvocReading = sensor.TVOC;
    auto eco2Reading = sensor.eCO2;

    Log::debug("sgp30-service.eco2-base", String(eco2BaseReading));
    Log::debug("sgp30-service.tvoc-base", String(tvocBaseReading));
    Log::debug("sgp30-service.h2", String(h2Reading));
    Log::debug("sgp30-service.ethanol", String(ethanolReading));
    Log::debug("sgp30-service.tvoc", String(tvocReading));
    Log::debug("sgp30-service.eco2", String(eco2Reading));

    auto eco2BaseResult = reinterpret_cast<uint8_t*>(&eco2BaseReading);
    auto tvocBaseResult = reinterpret_cast<uint8_t*>(&tvocBaseReading);
    auto h2Result = reinterpret_cast<uint8_t*>(&h2Reading);
    auto ethanolResult = reinterpret_cast<uint8_t*>(&ethanolReading);
    auto tvocResult = reinterpret_cast<uint8_t*>(&tvocReading);
    auto eco2Result = reinterpret_cast<uint8_t*>(&eco2Reading);

    response.insert(
      response.end(),
      eco2BaseResult,
      eco2BaseResult + sizeof(eco2BaseReading)
    );

    response.insert(
      response.end(),
      tvocBaseResult,
      tvocBaseResult + sizeof(tvocBaseReading)
    );

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

    Log::debug("sgp30-service", "sending response");

    respondCallback(response);
    respondCallback = NULL;

    vTaskDelete(NULL);
  }

  void handler(UDP::Payload *request, UDP::RespondCallback respond) {
    Log::debug("sgp30-service", "got request");

    if (!working) {
      Log::debug("sgp30-service", "sensor not working, sending null response");

      respond({});
      return;
    }

    respondCallback = respond;
    xTaskCreatePinnedToCore(
      responseTask,
      "sgp30_handling",
      2048,
      (void *)request,
      1,
      NULL,
      CONFIG_ARDUINO_RUNNING_CORE
    );
  }
}

} // project namespace

#endif
