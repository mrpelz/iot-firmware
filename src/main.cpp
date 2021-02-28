#include "./main.h"

void setup() {
  IotNode::Log::setup();

  auto udp = IotNode::UDP::setup();
  IotNode::Link::setup(udp);

  IotNode::Async::setup(udp);
  IotNode::Hello::setup(udp);
  IotNode::Keepalive::setup(udp);
  IotNode::SystemInfo::setup(udp);

  #ifdef IOT_NODE_BME280
    IotNode::Bme280::setup(udp);
  #endif

  #ifdef IOT_NODE_MCP9808
    IotNode::Mcp9808::setup(udp);
  #endif

  #ifdef IOT_NODE_TSL2561
    IotNode::Tsl2561::setup(udp);
  #endif

  auto buttons = IotNode::Button::setup();
  IotNode::Relais::setup(udp, buttons);
}

void loop() {
  yield();
}
