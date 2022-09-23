#include "./setup.h"

namespace IotNode {
namespace Utils {

namespace Keepalive {
  Class keepalive(
    3600000, // one hour
    []() {
      ESP.restart();
    }
  );

  Class eventPeer(
    10000,
    []() {
      Utils::UDP::instance.removeEventPeer();
    }
  );

  #ifdef IOT_NODE_POWER_CYCLE_PROTECTION
    Class powerCycleProtection(
      30000,
      []() {
        Output::output0.setOn(true);
      }
    );
  #endif

  void update() {
    keepalive.update();
    eventPeer.update();

    #ifdef IOT_NODE_POWER_CYCLE_PROTECTION
      powerCycleProtection.update();
    #endif
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
    #ifdef IOT_NODE_ESP32
      xTaskCreatePinnedToCore(
        task,
        "keepalive_maintenance",
        2048,
        NULL,
        1,
        NULL,
        CONFIG_ARDUINO_RUNNING_CORE
      );
    #endif

    #ifdef IOT_NODE_POWER_CYCLE_PROTECTION
      powerCycleProtection.tick();
    #endif
  }
}

} // section namespace
} // project namespace
