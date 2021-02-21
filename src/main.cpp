#include "./main.h"

void setup() {
  IotNode::Log::setup();

  auto udp = IotNode::UDP::setup();
  IotNode::Link::setup(udp);
  IotNode::Keepalive::setup(udp);

  auto buttons = IotNode::Button::setup();
  IotNode::Relais::setup(udp, buttons);

  udp->addService(&IotNode::helloService);
  udp->addService(&IotNode::systemInfoService);
  udp->addService(&IotNode::asyncService);
  udp->addService(&IotNode::mcp9808Service);
  udp->addService(&IotNode::bme280Service);
  udp->addService(&IotNode::tsl2561Service);
}

void loop() {
  IotNode::asyncUpdate();
  yield();

  IotNode::tsl2561UpdateHandler.update();
  yield();
}
