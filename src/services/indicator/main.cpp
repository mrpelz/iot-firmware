#include "./main.h"

namespace IotNode {
namespace Services {

namespace Indicator {
  void handler(Utils::UDP::Payload *request, Utils::UDP::RespondCallback respond) {
    Utils::Log::debug("indicator-service", "got request");

    if (request->size() < 2) {
      Utils::Log::debug("indicator-service", "missing parameters");
    }

    auto index = request->at(0);
    auto cmd = request->at(1);

    auto indicator = &Utils::Indicator::rxdLed;
    if (index == 1) {
      indicator = &Utils::Indicator::txdLed;
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

    Utils::Log::debug("indicator-service", "sending response");

    respond({});
  }
}

} // section namespace
} // project namespace
