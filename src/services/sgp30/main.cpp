#ifdef IOT_NODE_SGP30

#include "./main.h"

namespace IotNode::Services::Sgp30
{
  Utils::UDP::RespondCallback respondCallback = NULL;
  TaskHandle_t taskHandle = NULL;

  float calibrationTemperature;
  float calibrationHumidity;

  bool working = false;
  auto sensor = Adafruit_SGP30();

  uint32_t getAbsoluteHumidity(float temperature, float humidity)
  {
    // approximation formula from Sensirion SGP30 Driver Integration chapter 3.15
    const float absoluteHumidity = 216.7f * ((humidity / 100.0f) * 6.112f * exp((17.62f * temperature) / (243.12f + temperature)) / (273.15f + temperature)); // [g/m^3]
    const uint32_t absoluteHumidityScaled = static_cast<uint32_t>(1000.0f * absoluteHumidity);                                                                // [mg/m^3]
    return absoluteHumidityScaled;
  }

  void initializer(TwoWire *i2c)
  {
#ifdef IOT_NODE_LOGGING
    Utils::Log::debug("sgp30-service: initializing sensor");
#endif

    working = sensor.begin(i2c);
    if (!working)
    {
#ifdef IOT_NODE_LOGGING
      Utils::Log::debug("sgp30-service: sensor initialization failed");
#endif

      return;
    }

    xTaskCreatePinnedToCore(
        responseTask,
        "sgp30_handling",
        FREERTOS_STACK_SIZE,
        NULL,
        tskIDLE_PRIORITY,
        &taskHandle,
        CONFIG_ARDUINO_RUNNING_CORE);
  }

  void responseTask(void *parameter)
  {
    for (;;)
    {
      if (respondCallback == NULL)
      {
        vTaskSuspend(NULL);
        continue;
      }

      Utils::I2C::claim();

      auto calibrationSuccess = sensor.setHumidity(getAbsoluteHumidity(calibrationTemperature, calibrationHumidity));
      if (!calibrationSuccess)
      {
#ifdef IOT_NODE_LOGGING
        Utils::Log::debug("sgp30-service: calibration not successful, sending null response");
#endif

        Utils::I2C::unclaim();

        respondCallback({});
        respondCallback = NULL;
        continue;
      }

      auto measurementRawSuccess = sensor.IAQmeasureRaw();
      auto measurementSuccess = sensor.IAQmeasure();

      Utils::I2C::unclaim();

      if (!measurementRawSuccess || !measurementSuccess)
      {
#ifdef IOT_NODE_LOGGING
        Utils::Log::debug("sgp30-service: measurement not successful, sending null response");
#endif

        respondCallback({});
        respondCallback = NULL;
        continue;
      }

      auto h2Reading = sensor.rawH2;
      auto ethanolReading = sensor.rawEthanol;
      auto tvocReading = sensor.TVOC;
      auto eco2Reading = sensor.eCO2;

#ifdef IOT_NODE_LOGGING
      Utils::Log::debug(fmt::format("sgp30-service.h2: {}", h2Reading));
      Utils::Log::debug(fmt::format("sgp30-service.ethanol: {}", ethanolReading));
      Utils::Log::debug(fmt::format("sgp30-service.tvoc: {}", tvocReading));
      Utils::Log::debug(fmt::format("sgp30-service.eco2: {}", eco2Reading));
#endif

      auto h2Result = reinterpret_cast<uint8_t *>(&h2Reading);
      auto ethanolResult = reinterpret_cast<uint8_t *>(&ethanolReading);
      auto tvocResult = reinterpret_cast<uint8_t *>(&tvocReading);
      auto eco2Result = reinterpret_cast<uint8_t *>(&eco2Reading);

      Utils::UDP::Payload response;

      response.insert(
          response.end(),
          h2Result,
          h2Result + sizeof(h2Reading));

      response.insert(
          response.end(),
          ethanolResult,
          ethanolResult + sizeof(ethanolReading));

      response.insert(
          response.end(),
          tvocResult,
          tvocResult + sizeof(tvocReading));

      response.insert(
          response.end(),
          eco2Result,
          eco2Result + sizeof(eco2Reading));

#ifdef IOT_NODE_LOGGING
      Utils::Log::debug("sgp30-service: sending response");
#endif

      respondCallback(response);
      respondCallback = NULL;
    }
  }

  void handler(Utils::UDP::Payload *request, Utils::UDP::RespondCallback respond, Utils::UDP::Peer peer)
  {
#ifdef IOT_NODE_LOGGING
    Utils::Log::debug("sgp30-service: got request");
#endif

    if (!working)
    {
#ifdef IOT_NODE_LOGGING
      Utils::Log::debug("sgp30-service: sensor not working, sending null response");
#endif

      respond({});
      return;
    }

    if (request->size() < (sizeof(float) * 2))
    {
#ifdef IOT_NODE_LOGGING
      Utils::Log::debug("sgp30-service: request does not contain temperature and humidity");
#endif

      respond({});
      return;
    }
    calibrationTemperature = ((float *)request->data())[0];
    calibrationHumidity = ((float *)request->data())[1];

#ifdef IOT_NODE_LOGGING
    Utils::Log::debug(fmt::format("sgp30-service.calibration-temperature: {}", calibrationTemperature));
    Utils::Log::debug(fmt::format("sgp30-service.calibration-humidity: {}", calibrationHumidity));
#endif

    respondCallback = respond;
    if (taskHandle != NULL)
      vTaskResume(taskHandle);
  }
}

#endif
