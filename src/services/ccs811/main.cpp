#ifdef IOT_NODE_CCS811

#include "./main.h"

namespace IotNode::Services::Ccs811
{
  Utils::UDP::RespondCallback respondCallback = NULL;
  TaskHandle_t taskHandle = NULL;

  float calibrationTemperature;
  float calibrationHumidity;

  bool working = false;
  auto sensor = Adafruit_CCS811();

  void initializer(TwoWire *i2c)
  {
#ifdef IOT_NODE_LOGGING
    Utils::Log::debug("ccs811-service: initializing sensor");
#endif

    working = sensor.begin(CCS811_ADDRESS, i2c);
    if (!working)
    {
#ifdef IOT_NODE_LOGGING
      Utils::Log::debug("ccs811-service: sensor initialization failed");
#endif

      return;
    }

    xTaskCreatePinnedToCore(
        responseTask,
        "ccs811_handling",
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

      sensor.setEnvironmentalData(calibrationHumidity, calibrationTemperature);

      if (sensor.available() && !sensor.readData())
      {
#ifdef IOT_NODE_LOGGING
        Utils::Log::debug("ccs811-service: measurement not successful, sending null response");
#endif

        Utils::I2C::unclaim();

        respondCallback({});
        respondCallback = NULL;
        continue;
      }

      auto temperatureReading = sensor.calculateTemperature();
      auto tvocReading = sensor.getTVOC();
      auto eco2Reading = sensor.geteCO2();

      Utils::I2C::unclaim();

#ifdef IOT_NODE_LOGGING
      Utils::Log::debug(fmt::format("ccs811-service.temperature: {}", temperatureReading));
      Utils::Log::debug(fmt::format("ccs811-service.tvoc: {}", tvocReading));
      Utils::Log::debug(fmt::format("ccs811-service.eco2: {}", eco2Reading));
#endif

      auto temperatureResult = reinterpret_cast<unsigned char *>(&temperatureReading);
      auto tvocResult = reinterpret_cast<unsigned char *>(&tvocReading);
      auto eco2Result = reinterpret_cast<unsigned char *>(&eco2Reading);

      Utils::UDP::Payload response;

      response.insert(
          response.end(),
          temperatureResult,
          temperatureResult + sizeof(temperatureReading));

      response.insert(
          response.end(),
          tvocResult,
          tvocResult + sizeof(tvocReading));

      response.insert(
          response.end(),
          eco2Result,
          eco2Result + sizeof(eco2Reading));

#ifdef IOT_NODE_LOGGING
      Utils::Log::debug("ccs811-service: sending response");
#endif

      respondCallback(response);
      respondCallback = NULL;
    }
  }

  void handler(Utils::UDP::Payload *request, Utils::UDP::RespondCallback respond, Utils::UDP::Peer peer)
  {
#ifdef IOT_NODE_LOGGING
    Utils::Log::debug("ccs811-service: got request");
#endif

    if (!working)
    {
#ifdef IOT_NODE_LOGGING
      Utils::Log::debug("ccs811-service: sensor not working, sending null response");
#endif

      respond({});
      return;
    }

    if (request->size() < (sizeof(float) * 2))
    {
#ifdef IOT_NODE_LOGGING
      Utils::Log::debug("ccs811-service: request does not contain temperature and humidity");
#endif

      respond({});
      return;
    }
    calibrationTemperature = ((float *)request->data())[0];
    calibrationHumidity = ((float *)request->data())[1];

#ifdef IOT_NODE_LOGGING
    Utils::Log::debug(fmt::format("ccs811-service.calibration-temperature: {}", calibrationTemperature));
    Utils::Log::debug(fmt::format("ccs811-service.calibration-humidity: {}", calibrationHumidity));
#endif

    respondCallback = respond;
    if (taskHandle != NULL)
      vTaskResume(taskHandle);
  }
}

#endif
