#include "./main.h"

namespace IotNode {
  Relais relais0({ 4, false });
  auto relais0Service = makeRelaisService(&relais0, 0);
}

using namespace IotNode;

void setup() {
  Log::setup();

  auto udp = IotNode::UDP::setup();
  Link::setup(udp);
  Button::setup(udp, &relais0);

  udp->addService(&helloService);
  udp->addService(&systemInfoService);
  udp->addService(&asyncService);
  udp->addService(&mcp9808Service);
  udp->addService(&bme280Service);
  udp->addService(&tsl2561Service);
  udp->addService(&keepAliveService);
  udp->addService(&relais0Service);

  relais0.init();
}

void loop() {
  timeoutUpdate();
  yield();

  asyncUpdate();
  yield();

  tsl2561UpdateHandler.update();
  yield();
}
