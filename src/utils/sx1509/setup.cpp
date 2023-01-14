#ifdef IOT_NODE_SX1509

#include "./setup.h"

#define CLOCK_DIV 4

namespace IotNode {
namespace Utils {

namespace Sx1509 {
  bool working = false;

  SX1509 io;

  void setup() {
    #ifdef IOT_NODE_LOGGING
      Utils::Log::debug("sx1509-service", "initializing io");
    #endif

    working = io.begin(SX1509_ADDRESS, I2C::bus);

    if (!working) {
      #ifdef IOT_NODE_LOGGING
        Utils::Log::debug("sx1509-service", "io initialization failed");
      #endif

      return;
    }

    io.clock(INTERNAL_CLOCK_2MHZ, CLOCK_DIV);
    
    io.pinMode(ROOM_SENSOR_INDICATOR_LED_R, ANALOG_OUTPUT);
    io.breathe(ROOM_SENSOR_INDICATOR_LED_R, 1000, 250, 250, 250);
    
    io.pinMode(ROOM_SENSOR_INDICATOR_LED_G, ANALOG_OUTPUT);
    io.breathe(ROOM_SENSOR_INDICATOR_LED_G, 500, 250, 250, 250, 128);
    
    io.pinMode(ROOM_SENSOR_INDICATOR_LED_B, ANALOG_OUTPUT);
    io.breathe(ROOM_SENSOR_INDICATOR_LED_B, 250, 250, 250, 250, 64);

    // io.pinMode(ROOM_SENSOR_BUZZER, ANALOG_OUTPUT);
    // io.blink(ROOM_SENSOR_BUZZER, 500, 500, 127);
  }
}

} // section namespace
} // project namespace

#endif
