#ifdef IOT_NODE_SDS011

#include "./main.h"

namespace IotNode::Services::Sds011
{
  Utils::UDP::RespondCallback respondCallback = NULL;
  TaskHandle_t taskHandle = NULL;

  auto sensor = SdsDustSensor(IOT_NODE_SDS011_SERIAL);

  void sleepSensor()
  {
    for (size_t i = 0; i < 10; i++)
    {
      auto result = sensor.sleep();
      vTaskDelay(pdMS_TO_TICKS(100));

      if (!result.isWorking())
        break;
    }
  }

  void initializer()
  {
#ifdef IOT_NODE_LOGGING
    Utils::Log::debug("sds011-service: initializing sensor");
#endif

    sensor.begin();
    vTaskDelay(pdMS_TO_TICKS(100));

#ifdef IOT_NODE_LOGGING
    Utils::Log::debug("sds011-service.sensor-firmware-version", sensor.queryFirmwareVersion().toString().c_str());
    Utils::Log::debug("sds011-service.sensor-reporting-mode", sensor.setQueryReportingMode().toString().c_str());
#endif

    sleepSensor();

    xTaskCreatePinnedToCore(
        responseTask,
        "sds011_handling",
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

      sensor.wakeup();
      vTaskDelay(pdMS_TO_TICKS(100));

      auto reading = sensor.queryPm();
      sleepSensor();

#ifdef IOT_NODE_LOGGING
      Utils::Log::debug("sds011-service.status", reading.statusToString().c_str());
#endif

      if (!reading.isOk())
      {
#ifdef IOT_NODE_LOGGING
        Utils::Log::debug("sds011-service: measurement not successful, sending null response");
#endif

        respondCallback({});
        respondCallback = NULL;
        continue;
      }

#ifdef IOT_NODE_LOGGING
      Utils::Log::debug(fmt::format("sds011-service.pm025: {}", reading.pm25));
      Utils::Log::debug(fmt::format("sds011-service.pm10: {}", reading.pm10));
#endif

      auto pm025 = reinterpret_cast<unsigned char *>(&reading.pm25);
      auto pm10 = reinterpret_cast<unsigned char *>(&reading.pm10);

      Utils::UDP::Payload response;

      response.insert(
          response.end(),
          pm025,
          pm025 + sizeof(reading.pm25));

      response.insert(
          response.end(),
          pm10,
          pm10 + sizeof(reading.pm10));

#ifdef IOT_NODE_LOGGING
      Utils::Log::debug("sds011-service: sending response");
#endif

      respondCallback(response);
      respondCallback = NULL;
    }
  }

  void handler(Utils::UDP::Payload *request, Utils::UDP::RespondCallback respond, Utils::UDP::Peer peer)
  {
#ifdef IOT_NODE_LOGGING
    Utils::Log::debug("sds011-service: got request");
#endif

    respondCallback = respond;
    if (taskHandle != NULL)
      vTaskResume(taskHandle);
  }
}

#endif
