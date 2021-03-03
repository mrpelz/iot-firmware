#include "./main.h"

const TickType_t watchdogDelay = 10 / portTICK_PERIOD_MS;

void setup() {
  IotNode::Log::setup();

  auto udp = IotNode::UDP::setup();
  IotNode::Link::setup(udp);

  IotNode::Async::setup(udp);
  IotNode::Hello::setup(udp);
  IotNode::Keepalive::setup(udp);
  IotNode::SystemInfo::setup(udp);

  auto i2c = IotNode::I2C::setup();

  #ifdef IOT_NODE_BME280
    IotNode::Bme280::setup(udp, i2c);
  #endif

  #ifdef IOT_NODE_MCP9808
    IotNode::Mcp9808::setup(udp, i2c);
  #endif

  #ifdef IOT_NODE_TSL2561
    IotNode::Tsl2561::setup(udp, i2c);
  #endif

  auto buttons = IotNode::Button::setup();
  IotNode::Relais::setup(udp, buttons);
}

void loop() {
  vTaskDelay(watchdogDelay);
}
