#ifdef IOT_NODE_SDS011

#include "./main.h"

namespace IotNode {
namespace Services {

namespace Sds011 {
  Utils::UDP::RespondCallback respondCallback = NULL;

  auto sensor = SdsDustSensor(Serial1);

  void initializer() {
    Utils::Log::debug("sds011-service", "initializing sensor");

    sensor.begin();
    vTaskDelay(LOG_DELAY / portTICK_PERIOD_MS);
    
    Utils::Log::debug("sds011-service.sensor-firmware-version", sensor.queryFirmwareVersion().toString());
    Utils::Log::debug("sds011-service.sensor-reporting-mode", sensor.setQueryReportingMode().toString());
    sensor.sleep();
  }

  void responseTask(void *parameter) {
    if (respondCallback == NULL) {
      vTaskDelete(NULL);
      return;
    }

    sensor.wakeup();
    vTaskDelay(30000 / portTICK_PERIOD_MS);

    auto reading = sensor.queryPm();
    sensor.sleep();

    Utils::Log::debug("sds011-service.status", reading.statusToString());

    if (!reading.isOk()) {
      Utils::Log::debug("sds011-service", "measurement not successful, sending null response");

      respondCallback({});
      respondCallback = NULL;

      vTaskDelete(NULL);
      return;
    }

    Utils::Log::debug("sds011-service.pm025", String(reading.pm25));
    Utils::Log::debug("sds011-service.pm10", String(reading.pm10));

    auto pm025 = reinterpret_cast<uint8_t*>(&reading.pm25);
    auto pm10 = reinterpret_cast<uint8_t*>(&reading.pm10);

    Utils::UDP::Payload response;

    response.insert(
      response.end(),
      pm025,
      pm025 + sizeof(reading.pm25)
    );

    response.insert(
      response.end(),
      pm10,
      pm10 + sizeof(reading.pm10)
    );

    Utils::Log::debug("sds011-service", "sending response");

    respondCallback(response);
    respondCallback = NULL;

    vTaskDelete(NULL);
  }

  void handler(Utils::UDP::Payload *request, Utils::UDP::RespondCallback respond) {
    Utils::Log::debug("sds011-service", "got request");

    if (respondCallback != NULL) return;

    respondCallback = respond;
    xTaskCreatePinnedToCore(
      responseTask,
      "sds011_handling",
      2048,
      NULL,
      1,
      NULL,
      CONFIG_ARDUINO_RUNNING_CORE
    );
  }
}

} // section namespace
} // project namespace

#endif
