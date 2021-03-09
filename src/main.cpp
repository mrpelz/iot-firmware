#include "./main.h"

void setup() {
  IotNode::Log::setup();

  IotNode::Indicator::setup();
  IotNode::Indicator::rxdLed.setOn(true);

  auto udp = IotNode::UDP::setup();
  IotNode::Link::setup(udp);

  #ifdef ARDUINO_ARCH_ESP32
    IotNode::Async::setup(udp);
  #endif

  IotNode::Hello::setup(udp);
  IotNode::Keepalive::setup(udp);
  IotNode::SystemInfo::setup(udp);

  #if defined(IOT_NODE_BME280) || defined(IOT_NODE_CCS811) || defined(IOT_NODE_MCP9808) || defined(IOT_NODE_SGP30) || defined(IOT_NODE_TSL2561) || defined(IOT_NODE_VEML6070) || defined(IOT_NODE_I2C_SCAN)
    auto i2c = IotNode::I2C::setup();

    #ifdef IOT_NODE_I2C_SCAN
      IotNode::I2C::scan();
    #endif
  #endif

  #ifdef ARDUINO_ARCH_ESP32
    #ifdef IOT_NODE_BME280
      IotNode::Bme280::setup(udp, i2c);
    #endif

    #ifdef IOT_NODE_CCS811
      IotNode::Ccs811::setup(udp, i2c);
    #endif

    #ifdef IOT_NODE_MCP9808
      IotNode::Mcp9808::setup(udp, i2c);
    #endif

    #ifdef IOT_NODE_SGP30
      IotNode::Sgp30::setup(udp, i2c);
    #endif

    #ifdef IOT_NODE_TSL2561
      IotNode::Tsl2561::setup(udp, i2c);
    #endif

    #ifdef IOT_NODE_VEML6070
      IotNode::Veml6070::setup(udp, i2c);
    #endif
  #endif

  auto buttons = IotNode::Button::setup();
  IotNode::Relais::setup(udp, buttons);

  IotNode::Indicator::rxdLed.blink();
}

void loop() {
  #ifdef ARDUINO_ARCH_ESP8266
    delay(WATCHDOG_DELAY);
    IotNode::Indicator::update();
    IotNode::Button::update();
    IotNode::Keepalive::update();
    IotNode::Link::update();
  #endif
  #ifdef ARDUINO_ARCH_ESP32
    vTaskDelay(WATCHDOG_DELAY / portTICK_PERIOD_MS);
  #endif
}
