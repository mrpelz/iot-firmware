#include "./setup.h"

#ifdef IOT_NODE_RF_433

namespace IotNode {
namespace Events {

namespace Rf433 {
  auto rf = RCSwitch();

  void update() {
    if (rf.available()) {
      auto protocol = rf.getReceivedProtocol();
      auto bitLength = rf.getReceivedBitlength();
      auto value = rf.getReceivedValue();

      #ifdef IOT_NODE_LOGGING
        Utils::Log::debug("event.rf433.protocol", String(protocol));
        Utils::Log::debug("event.rf433.delay", String(rf.getReceivedDelay()));
        Utils::Log::debug("event.rf433.length", String(bitLength));
        Utils::Log::debug("event.rf433.value", String(value));
      #endif

      onDataReceived(&protocol, &bitLength, &value);

      rf.resetAvailable();
    }
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
    rf.enableReceive(digitalPinToInterrupt(4));

    #ifdef IOT_NODE_ESP32
      xTaskCreatePinnedToCore(
        task,
        "rf433_maintenance",
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

#endif
