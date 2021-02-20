#include "./main.h"

namespace IotNode {
  Relais relais0({ 4, false });
  auto relais0Service = makeRelaisService(&relais0, 0);

  Buttons buttons(buttonsConfig);
}

using namespace IotNode;

void setup() {
  Log::setup();

  auto udp = IotNode::UDP::setup();

  Link::setup(udp);

  udp->addService(&helloService);
  udp->addService(&systemInfoService);
  udp->addService(&asyncService);
  udp->addService(&mcp9808Service);
  udp->addService(&bme280Service);
  udp->addService(&tsl2561Service);
  udp->addService(&keepAliveService);
  udp->addService(&relais0Service);

  buttons.setDebug(Log::debug);

  buttons.setChangeCallback([udp](ButtonUpdate update) {
    if (udp->isListening() && udp->hasEventPeer()) {
      buttonEvent(udp, update);
      return;
    }

    Log::debug("info.buttons.change-callback", "udp event not usable");

    if (
      update.index == 0
      && update.downChanged
      && !update.down
    ) {
      Log::debug("info.buttons.change-callback", "triggering override");
      relais0.toggle();
    }
  });

  relais0.init();
  buttons.start();
}

void loop() {
  timeoutUpdate();
  yield();

  asyncUpdate();
  yield();

  buttons.update();
  yield();

  tsl2561UpdateHandler.update();
  yield();
}
