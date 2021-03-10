#include "./main.h"

void setup() {
  IotNode::Utils::Log::setup();

  IotNode::Utils::Indicator::setup();
  IotNode::Utils::Indicator::rxdLed.setOn(true);

  auto udp = IotNode::Utils::UDP::setup();
  IotNode::Utils::Link::setup(udp);

  #ifdef ARDUINO_ARCH_ESP32
    IotNode::Services::Async::setup(udp);
  #endif

  IotNode::Services::Indicator::setup(udp);
  IotNode::Services::Hello::setup(udp);
  IotNode::Services::Keepalive::setup(udp);
  IotNode::Services::SystemInfo::setup(udp);

  #if defined(IOT_NODE_BME280) || defined(IOT_NODE_CCS811) || defined(IOT_NODE_MCP9808) || defined(IOT_NODE_SGP30) || defined(IOT_NODE_TSL2561) || defined(IOT_NODE_VEML6070) || defined(IOT_NODE_I2C_SCAN)
    auto i2c = IotNode::Utils::I2C::setup();

    #ifdef IOT_NODE_I2C_SCAN
      IotNode::Utils::I2C::scan();
    #endif
  #endif

  #ifdef ARDUINO_ARCH_ESP32
    #ifdef IOT_NODE_BME280
      IotNode::Services::Bme280::setup(udp, i2c);
    #endif

    #ifdef IOT_NODE_CCS811
      IotNode::Services::Ccs811::setup(udp, i2c);
    #endif

    #ifdef IOT_NODE_MCP9808
      IotNode::Services::Mcp9808::setup(udp, i2c);
    #endif

    #ifdef IOT_NODE_SGP30
      IotNode::Services::Sgp30::setup(udp, i2c);
    #endif

    #ifdef IOT_NODE_TSL2561
      IotNode::Services::Tsl2561::setup(udp, i2c);
    #endif

    #ifdef IOT_NODE_VEML6070
      IotNode::Services::Veml6070::setup(udp, i2c);
    #endif
  #endif

  auto buttons = IotNode::Events::Button::setup();
  IotNode::Utils::Relais::setup(udp, buttons);
  IotNode::Services::Relais::setup(udp);

  IotNode::Utils::Indicator::rxdLed.blink();
}

void loop() {
  #ifdef ARDUINO_ARCH_ESP8266
    delay(WATCHDOG_DELAY);
    IotNode::Indicator::update();
    IotNode::Events::Button::update();
    IotNode::Services::Keepalive::update();
    IotNode::Link::update();
  #endif
  #ifdef ARDUINO_ARCH_ESP32
    vTaskDelay(WATCHDOG_DELAY / portTICK_PERIOD_MS);
  #endif
}
