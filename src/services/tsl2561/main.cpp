#ifdef IOT_NODE_TSL2561

#include "./main.h"

namespace IotNode
{
  namespace Services
  {

    namespace Tsl2561
    {
      Utils::UDP::RespondCallback respondCallback = NULL;
      TaskHandle_t taskHandle = NULL;

      bool working = false;
      auto sensor = Sensor();

      void initializer(TwoWire *i2c)
      {
#ifdef IOT_NODE_LOGGING
        Utils::Log::debug("tsl2561-service: initializing sensor");
#endif

        working = sensor.begin(i2c);

        if (!working)
        {
#ifdef IOT_NODE_LOGGING
          Utils::Log::debug("tsl2561-service: sensor initialization failed");
#endif

          return;
        }

        sensor.enableAutoRange(true);
        sensor.setIntegrationTime(TSL2561_INTEGRATIONTIME_402MS);

        xTaskCreatePinnedToCore(
            responseTask,
            "tsl2561_handling",
            2048,
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

          uint16_t broadband;
          uint16_t infrared;

          Utils::I2C::claim();

          sensor.getLuminosity(&broadband, &infrared);
          auto reading = sensor.calculateLux(broadband, infrared);

          Utils::I2C::unclaim();

#ifdef IOT_NODE_LOGGING
          Utils::Log::debug(fmt::format("tsl2561-service.reading: {}", reading));
#endif

          auto result = reinterpret_cast<uint8_t *>(&reading);

          Utils::UDP::Payload response;

          response.insert(response.end(), result, result + sizeof(reading));

#ifdef IOT_NODE_LOGGING
          Utils::Log::debug("tsl2561-service: sending response");
#endif

          respondCallback(response);
          respondCallback = NULL;
        }
      }

      void handler(Utils::UDP::Payload *request, Utils::UDP::RespondCallback respond, Utils::UDP::Peer peer)
      {
#ifdef IOT_NODE_LOGGING
        Utils::Log::debug("tsl2561-service: got request");
#endif

        if (!working)
        {
#ifdef IOT_NODE_LOGGING
          Utils::Log::debug("tsl2561-service: sensor not working, sending null response");
#endif

          respond({});
          return;
        }

        respondCallback = respond;
        if (taskHandle != NULL)
          vTaskResume(taskHandle);
      }
    }

  } // section namespace
} // project namespace

#endif
