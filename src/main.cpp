#include "./main.h"

void setup() {
  IotNode::Log::setup();

  auto udp = IotNode::UDP::setup();
  IotNode::Link::setup(udp);

  IotNode::Async::setup(udp);
  IotNode::Hello::setup(udp);
  IotNode::Keepalive::setup(udp);

  auto buttons = IotNode::Button::setup();
  IotNode::Relais::setup(udp, buttons);

  udp->addService(&IotNode::systemInfoService);
  udp->addService(&IotNode::mcp9808Service);
  udp->addService(&IotNode::bme280Service);
  udp->addService(&IotNode::tsl2561Service);
}

void loop() {
  IotNode::tsl2561UpdateHandler.update();
  yield();
}
