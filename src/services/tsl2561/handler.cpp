#include "./handler.h"

namespace IotNode {

UpdateHandler makeTsl2561UpdateHandler() {
  auto responseCallback = std::make_shared<std::function<void (std::vector<uint8_t> response)>>();

  auto working = std::make_shared<bool>(false);
  auto requestTime = std::make_shared<uint32_t>(0);
  auto requestRunning = std::make_shared<bool>(false);

  auto sensor = std::make_shared<SFE_TSL2561>(SFE_TSL2561());
  auto ms = std::make_shared<uint32_t>();

  auto initializer = [
    &,
    ms,
    sensor,
    working
  ]() {
    if (*working) return;

    Log::debug("tsl2561-service", "initializing sensor");

    uint8_t id;

    sensor->begin();
    *working = sensor->getID(id);

    if (!*working) {
      Log::debug("tsl2561-service", "sensor initialization failed");
      return;
    }

    sensor->setTiming(false, 2, *ms);
  };

  auto handler = [
    &,
    requestRunning,
    requestTime,
    responseCallback,
    sensor,
    working
  ](
    std::vector<uint8_t> *request,
    std::function<void (std::vector<uint8_t> response)> respond
  ) {
    Log::debug("tsl2561-service", "got request");

    initializer();

    if (!*working) {
      Log::debug("tsl2561-service", "sensor not working, sending null response");

      respond({});
      return;
    }

    *requestTime = millis();
    *requestRunning = true;
    sensor->setPowerUp();

    *responseCallback = [respond](std::vector<uint8_t> response) {
      respond(response);
    };
  };

  auto update = [
    &,
    ms,
    requestRunning,
    requestTime,
    responseCallback,
    sensor,
    working
  ]() {
    if (!*requestRunning) return;

    uint32_t timeSinceRequest = millis() - *requestTime;
    if (timeSinceRequest < *ms) return;
    *requestTime = 0;
    *requestRunning = false;

    if (!*working) {
      (*responseCallback)({});
      return;
    }

    uint32_t data0, data1;
    double lux;

    sensor->getData(data0, data1);
    bool good = sensor->getLux(2, *ms, data0, data1, lux);

    sensor->setPowerDown();

    if (!good) {
      (*responseCallback)({});
      return;
    }

    std::vector<uint8_t> response;

    response.insert(response.end(), &lux, &lux + sizeof(lux));

    Log::debug("tsl2561-service", "sending delayed response");

    (*responseCallback)(response);
  };

  return {
    handler,
    update
  };
}

} // project namespace
