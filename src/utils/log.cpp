#include "./log.h"

#ifdef IOT_NODE_LOGGING

namespace IotNode::Utils::Log
{
  void setup()
  {
    Serial.begin(IOT_NODE_SERIAL0_BAUD_RATE);

#if defined(IOT_NODE_ESP8266) && defined(IOT_NODE_BOARD_H801)
    Serial.set_tx(2);
#endif

#ifdef IOT_NODE_ESP8266
    delay(IOT_NODE_LOG_DELAY);
#endif
#ifdef IOT_NODE_ESP32
    vTaskDelay(IOT_NODE_LOG_DELAY / portTICK_PERIOD_MS);
#endif

    info();
  }

  void debug(::std::string key, ::std::string value)
  {
    auto line = fmt::format("[{}]:\"{}\":\"{}\"\n", millis(), key, value);
    Serial.write(line.c_str());
  }
  void debug(::std::string value)
  {
    auto line = fmt::format("[{}]:\"{}\"\n", millis(), value);
    Serial.write(line.c_str());
  }

  void info()
  {
    yield();

    Serial.println();

    debug("info.name", IOT_NODE_NAME);

#ifdef IOT_NODE_BOARD_NAME
    debug("info.board-name", IOT_NODE_BOARD_NAME);
#endif

#ifdef IOT_NODE_HARDWARE_NAME
    debug("info.hardware-name", IOT_NODE_HARDWARE_NAME);
#endif

    debug("info.build.git-rev", IOT_NODE_BUILD_GIT_REV);
    debug("info.build.pio.env", IOT_NODE_PIO_ENV);
    debug("info.build.pio.platform", IOT_NODE_PIO_PLATFORM);
    debug("info.build.pio.framework", IOT_NODE_PIO_FRAMEWORK);

#ifdef IOT_NODE_ESP8266
    log_e("info.system.chip-id", String(ESP.getChipId(), HEX).c_str());
    log_e("info.system.flash-id", String(ESP.getFlashChipId(), HEX).c_str());
#endif

    debug("info.system.mac-address", WiFi.macAddress().c_str());
  }
}

#endif
