#include "./ota.h"

namespace IotNode {
namespace Utils {

namespace OTA {
  void update() {
    ArduinoOTA.handle();
  }

  #ifdef IOT_NODE_ESP32
    void task(void *parameter) {
      for(;;) {
        update();
        vTaskDelay(IOT_NODE_MUTLITASKING_DELAY / portTICK_PERIOD_MS);
      }
    }
  #endif

  void setup() {
    ArduinoOTA.setHostname(IOT_NODE_NAME);
    ArduinoOTA.setPassword(IOT_NODE_OTA_PASSWORD);

    ArduinoOTA.onStart([]() {
      Log::debug("ota", "start");
    });

    ArduinoOTA.onEnd([]() {
      Log::debug("ota", "end");
    });

    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
      Log::debug("ota.progress", String(progress / (total / 100)));
    });

    ArduinoOTA.onError([](ota_error_t error) {
      if (error == OTA_AUTH_ERROR) Log::debug("ota.error", "auth");
      else if (error == OTA_BEGIN_ERROR) Log::debug("ota.error", "begin");
      else if (error == OTA_CONNECT_ERROR) Log::debug("ota.error", "connect");
      else if (error == OTA_RECEIVE_ERROR) Log::debug("ota.error", "receive");
      else if (error == OTA_END_ERROR) Log::debug("ota.error", "end");
    });

    #ifdef IOT_NODE_ESP32
      xTaskCreatePinnedToCore(
        task,
        "ota_maintenance",
        2048,
        NULL,
        3,
        NULL,
        CONFIG_ARDUINO_RUNNING_CORE
      );
    #endif
  }
}

} // section namespace
} // project namespace
