#ifdef IOT_NODE_SDS011

#include "./main.h"

namespace IotNode
{
  namespace Services
  {

    namespace Sds011
    {
      Utils::UDP::RespondCallback respondCallback = NULL;
      TaskHandle_t taskHandle = NULL;

      auto sensor = SdsDustSensor(IOT_NODE_SDS011_SERIAL);

      void sleepSensor()
      {
        for (size_t i = 0; i < 10; i++)
        {
          auto result = sensor.sleep();
          vTaskDelay(IOT_NODE_LOG_DELAY / portTICK_PERIOD_MS);

          if (!result.isWorking())
            break;
        }
      }

      void initializer()
      {
#ifdef IOT_NODE_LOGGING
        Utils::Log::debug("sds011-service", "initializing sensor");
#endif

        sensor.begin();
        vTaskDelay(IOT_NODE_LOG_DELAY / portTICK_PERIOD_MS);

#ifdef IOT_NODE_LOGGING
        Utils::Log::debug("sds011-service.sensor-firmware-version", sensor.queryFirmwareVersion().toString());
        Utils::Log::debug("sds011-service.sensor-reporting-mode", sensor.setQueryReportingMode().toString());
#endif

        sleepSensor();

        xTaskCreatePinnedToCore(
            responseTask,
            "sds011_handling",
            2048,
            NULL,
            1,
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
          vTaskDelay(30000 / portTICK_PERIOD_MS);

          auto reading = sensor.queryPm();
          sleepSensor();

#ifdef IOT_NODE_LOGGING
          Utils::Log::debug("sds011-service.status", reading.statusToString());
#endif

          if (!reading.isOk())
          {
#ifdef IOT_NODE_LOGGING
            Utils::Log::debug("sds011-service", "measurement not successful, sending null response");
#endif

            respondCallback({});
            respondCallback = NULL;
            continue;
          }

#ifdef IOT_NODE_LOGGING
          Utils::Log::debug("sds011-service.pm025", String(reading.pm25));
          Utils::Log::debug("sds011-service.pm10", String(reading.pm10));
#endif

          auto pm025 = reinterpret_cast<uint8_t *>(&reading.pm25);
          auto pm10 = reinterpret_cast<uint8_t *>(&reading.pm10);

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
          Utils::Log::debug("sds011-service", "sending response");
#endif

          respondCallback(response);
          respondCallback = NULL;
        }
      }

      void handler(Utils::UDP::Payload *request, Utils::UDP::RespondCallback respond, Utils::UDP::Peer peer)
      {
#ifdef IOT_NODE_LOGGING
        Utils::Log::debug("sds011-service", "got request");
#endif

        respondCallback = respond;
        if (taskHandle != NULL)
          vTaskResume(taskHandle);
      }
    }

  } // section namespace
} // project namespace

#endif
