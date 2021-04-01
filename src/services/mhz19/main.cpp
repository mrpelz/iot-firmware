#ifdef IOT_NODE_MHZ19

#include "./main.h"

namespace IotNode {
namespace Services {

namespace Mhz19 {
  Utils::UDP::RespondCallback respondCallback = NULL;
  TaskHandle_t taskHandle = NULL;

  auto sensor = MHZ19();

  void initializer() {
    Utils::Log::debug("mhz19-service", "initializing sensor");

    Serial2.begin(9600);
    vTaskDelay(LOG_DELAY / portTICK_PERIOD_MS);

    sensor.begin(Serial2);
    sensor.autoCalibration();

    xTaskCreatePinnedToCore(
      responseTask,
      "mhz19_handling",
      2048,
      NULL,
      1,
      &taskHandle,
      CONFIG_ARDUINO_RUNNING_CORE
    );
  }

  void responseTask(void *parameter) {
    for(;;) {
      if (respondCallback == NULL) {
        vTaskSuspend(NULL);
        continue;
      }

      auto accuracyReading = sensor.getAccuracy();
      auto abcReading = sensor.getABC();
      auto co2Reading = sensor.getCO2();
      auto temperatureReading = sensor.getTemperature();
      auto transmittanceReading = sensor.getTransmittance();

      Utils::Log::debug("mhz19-service.accuracy", String(accuracyReading));
      Utils::Log::debug("mhz19-service.abc", String(abcReading));
      Utils::Log::debug("mhz19-service.co2", String(co2Reading));
      Utils::Log::debug("mhz19-service.temperature", String(temperatureReading));
      Utils::Log::debug("mhz19-service.transmittance", String(transmittanceReading));

      auto accuracy = reinterpret_cast<uint8_t*>(&accuracyReading);
      auto abc = reinterpret_cast<uint8_t*>(&abcReading);
      auto co2 = reinterpret_cast<uint8_t*>(&co2Reading);
      auto temperature = reinterpret_cast<uint8_t*>(&temperatureReading);
      auto transmittance = reinterpret_cast<uint8_t*>(&transmittanceReading);

      Utils::UDP::Payload response;

      response.insert(
        response.end(),
        accuracy,
        accuracy + sizeof(accuracyReading)
      );
      
      response.insert(
        response.end(),
        abc,
        abc + sizeof(abcReading)
      );

      response.insert(
        response.end(),
        co2,
        co2 + sizeof(co2Reading)
      );

      response.insert(
        response.end(),
        temperature,
        temperature + sizeof(temperatureReading)
      );

      response.insert(
        response.end(),
        transmittance,
        transmittance + sizeof(transmittanceReading)
      );


      Utils::Log::debug("mhz19-service", "sending response");

      respondCallback(response);
      respondCallback = NULL;
    }
  }

  void handler(Utils::UDP::Payload *request, Utils::UDP::RespondCallback respond) {
    Utils::Log::debug("mhz19-service", "got request");

    respondCallback = respond;
    if (taskHandle != NULL) vTaskResume(taskHandle);
  }
}

} // section namespace
} // project namespace

#endif
