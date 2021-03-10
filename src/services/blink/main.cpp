#include "./main.h"

namespace IotNode {

namespace Blink {
  void handler(UDP::Payload *request, UDP::RespondCallback respond) {
    Log::debug("blink-service", "got request");

    if (request->size() < 2) {
      Log::debug("blink-service", "missing parameters");
    }

    auto index = request->at(0);
    auto cmd = request->at(1);

    auto indicator = &Indicator::rxdLed;
    if (index == 1) {
      indicator = &Indicator::txdLed;
    }

    if (cmd == 0) {
      indicator->setOn(false);
    } else if (cmd == 1) {
      if (request->size() >= 3) {
        auto count = request->at(2);
        indicator->blink(count);
      } else {
        indicator->blink();
      }
    }

    Log::debug("blink-service", "sending response");

    respond({});
  }
}

} // project namespace
