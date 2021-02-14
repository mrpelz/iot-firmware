#include "./handler.h"

UpdateHandler makeTsl2561UpdateHandler() {
  std::function<void (std::vector<uint8_t> response)> responseCallback;

  bool working = false;
  bool requestRunning = false;
  uint32_t requestTime = 0;

  SFE_TSL2561 sensor;
  uint32_t ms;

  auto initializer = [&]() {
    if (working) return;

    debug("tsl2561-service", "initializing sensor");

    uint8_t id;

    sensor.begin();
    working = sensor.getID(id);

    if (!working) {
      debug("tsl2561-service", "sensor initialization failed");
      return;
    }

    sensor.setTiming(false, 2, ms);
  };

  auto handler = [&](
    std::vector<uint8_t> *request,
    std::function<void (std::vector<uint8_t> response)> respond
  ) {
    debug("tsl2561-service", "got request");

    initializer();

    if (!working) {
      debug("tsl2561-service", "sensor not working, sending null response");

      respond({});
      return;
    }

    requestTime = millis();
    requestRunning = true;
    sensor.setPowerUp();

    responseCallback = [respond](std::vector<uint8_t> response) {
      respond(response);
    };
  };

  auto update = [&]() {
    if (!requestRunning) return;

    uint32_t timeSinceRequest = millis() - requestTime;
    if (timeSinceRequest < ms) return;
    requestTime = 0;
    requestRunning = false;

    if (!working) {
      responseCallback({});
      return;
    }

    uint32_t data0, data1;
    double lux;
    bool good;

    sensor.getData(data0, data1);
    good = sensor.getLux(2,ms,data0,data1,lux);

    sensor.setPowerDown();

    if (!good) {
      responseCallback({});
      return;
    }

    std::vector<uint8_t> response;

    response.insert(response.end(), &lux, &lux + sizeof(lux));

    debug("tsl2561-service", "sending delayed response");

    responseCallback(response);
  };

  return {
    handler,
    update
  };
}
