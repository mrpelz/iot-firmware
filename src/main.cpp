#include "./main.h"

void setup() {
  IotNode::Log::setup();

  auto udp = IotNode::UDP::setup();
  IotNode::Link::setup(udp);

  #ifdef ARDUINO_ARCH_ESP32
    IotNode::Async::setup(udp);
  #endif

  IotNode::Hello::setup(udp);
  IotNode::Keepalive::setup(udp);
  IotNode::SystemInfo::setup(udp);

  #if defined(IOT_NODE_BME280) || defined(IOT_NODE_MCP9808) || defined(IOT_NODE_TSL2561)
    auto i2c = IotNode::I2C::setup();
  #endif

  #ifdef ARDUINO_ARCH_ESP32
    #ifdef IOT_NODE_BME280
      IotNode::Bme280::setup(udp, i2c);
    #endif

    #ifdef IOT_NODE_MCP9808
      IotNode::Mcp9808::setup(udp, i2c);
    #endif

    #ifdef IOT_NODE_TSL2561
      IotNode::Tsl2561::setup(udp, i2c);
    #endif
  #endif

  auto buttons = IotNode::Button::setup();
  IotNode::Relais::setup(udp, buttons);
}

void loop() {
  #ifdef ARDUINO_ARCH_ESP8266
    delay(WATCHDOG_DELAY);
    IotNode::Button::update();
    IotNode::Keepalive::update();
    IotNode::Link::update();
  #endif
  #ifdef ARDUINO_ARCH_ESP32
    vTaskDelay(WATCHDOG_DELAY / portTICK_PERIOD_MS);
  #endif
}
