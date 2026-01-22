#ifdef IOT_NODE_MCP9808

#include "./main.h"

namespace IotNode
{
  namespace Services
  {

    namespace Mcp9808
    {
      Utils::UDP::RespondCallback respondCallback = NULL;
      TaskHandle_t taskHandle = NULL;

      bool working = false;
      auto sensor = Adafruit_MCP9808();

      void initializer(TwoWire *i2c)
      {
#ifdef IOT_NODE_LOGGING
        Utils::Log::debug("mcp9808-service", "initializing sensor");
#endif

        working = sensor.begin(MCP9808_I2CADDR_DEFAULT, i2c);
        if (!working)
        {
#ifdef IOT_NODE_LOGGING
          Utils::Log::debug("mcp9808-service", "sensor initialization failed");
#endif

          return;
        }

        sensor.setResolution(3);
        sensor.shutdown();

        xTaskCreatePinnedToCore(
            responseTask,
            "mcp9808_handling",
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

          Utils::I2C::claim();

          sensor.wake();

          auto reading = sensor.readTempC();

          sensor.shutdown();

          Utils::I2C::unclaim();

#ifdef IOT_NODE_LOGGING
          Utils::Log::debug("mcp9808-service.reading", String(reading));
#endif

          auto result = reinterpret_cast<uint8_t *>(&reading);

          Utils::UDP::Payload response;

          response.insert(
              response.end(),
              result,
              result + sizeof(reading));

#ifdef IOT_NODE_LOGGING
          Utils::Log::debug("mcp9808-service", "sending response");
#endif

          respondCallback(response);
          respondCallback = NULL;
        }
      }

      void handler(Utils::UDP::Payload *request, Utils::UDP::RespondCallback respond, Utils::UDP::Peer peer)
      {
#ifdef IOT_NODE_LOGGING
        Utils::Log::debug("mcp9808-service", "got request");
#endif

        if (!working)
        {
#ifdef IOT_NODE_LOGGING
          Utils::Log::debug("mcp9808-service", "sensor not working, sending null response");
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
