#include "./main.h"

void setup() {
  IotNode::Log::setup();

  auto udp = IotNode::UDP::setup();
  IotNode::Link::setup(udp);

  IotNode::Async::setup(udp);
  IotNode::Bme280::setup(udp);
  IotNode::Hello::setup(udp);
  IotNode::Keepalive::setup(udp);
  IotNode::Mcp9808::setup(udp);
  IotNode::SystemInfo::setup(udp);
  IotNode::Tsl2561::setup(udp);

  auto buttons = IotNode::Button::setup();
  IotNode::Relais::setup(udp, buttons);
}

void loop() {
  yield();
}
