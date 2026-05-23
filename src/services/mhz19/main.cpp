#ifdef IOT_NODE_MHZ19

#include "./main.h"

namespace IotNode
{
  namespace Services
  {

    namespace Mhz19
    {
      Utils::UDP::RespondCallback respondCallback = NULL;
      TaskHandle_t taskHandle = NULL;

      auto sensor = MHZ19();

      void initializer()
      {
#ifdef IOT_NODE_LOGGING
        Utils::Log::debug("mhz19-service: initializing sensor");
#endif

        IOT_NODE_MHZ19_SERIAL.begin(9600);
        vTaskDelay(IOT_NODE_LOG_DELAY / portTICK_PERIOD_MS);

        sensor.begin(IOT_NODE_MHZ19_SERIAL);
        sensor.autoCalibration();

        xTaskCreatePinnedToCore(
            responseTask,
            "mhz19_handling",
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

          auto accuracyReading = sensor.getAccuracy();
          auto abcReading = sensor.getABC();
          auto co2Reading = sensor.getCO2();
          auto temperatureReading = sensor.getTemperature();
          auto transmittanceReading = sensor.getTransmittance();

#ifdef IOT_NODE_LOGGING
          Utils::Log::debug(fmt::format("mhz19-service.accuracy: {}", accuracyReading));
          Utils::Log::debug(fmt::format("mhz19-service.abc: {}", abcReading));
          Utils::Log::debug(fmt::format("mhz19-service.co2: {}", co2Reading));
          Utils::Log::debug(fmt::format("mhz19-service.temperature: {}", temperatureReading));
          Utils::Log::debug(fmt::format("mhz19-service.transmittance: {}", transmittanceReading));
#endif

          auto accuracy = reinterpret_cast<uint8_t *>(&accuracyReading);
          auto abc = reinterpret_cast<uint8_t *>(&abcReading);
          auto co2 = reinterpret_cast<uint8_t *>(&co2Reading);
          auto temperature = reinterpret_cast<uint8_t *>(&temperatureReading);
          auto transmittance = reinterpret_cast<uint8_t *>(&transmittanceReading);

          Utils::UDP::Payload response;

          response.insert(
              response.end(),
              accuracy,
              accuracy + sizeof(accuracyReading));

          response.insert(
              response.end(),
              abc,
              abc + sizeof(abcReading));

          response.insert(
              response.end(),
              co2,
              co2 + sizeof(co2Reading));

          response.insert(
              response.end(),
              temperature,
              temperature + sizeof(temperatureReading));

          response.insert(
              response.end(),
              transmittance,
              transmittance + sizeof(transmittanceReading));

#ifdef IOT_NODE_LOGGING
          Utils::Log::debug("mhz19-service: sending response");
#endif

          respondCallback(response);
          respondCallback = NULL;
        }
      }

      void handler(Utils::UDP::Payload *request, Utils::UDP::RespondCallback respond, Utils::UDP::Peer peer)
      {
#ifdef IOT_NODE_LOGGING
        Utils::Log::debug("mhz19-service: got request");
#endif

        respondCallback = respond;
        if (taskHandle != NULL)
          vTaskResume(taskHandle);
      }
    }

  } // section namespace
} // project namespace

#endif
