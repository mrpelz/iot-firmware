#include "./handler.h"

UpdateHandler makeTsl2561UpdateHandler() {
  std::function<void (std::vector<uint8_t> response)> responseCallback;

  bool working = false;
  unsigned long requestTime = 0;
  SFE_TSL2561 sensor;
  uint32_t ms;

  auto initializer = [&]() {
    if (working) return;

    uint8_t id;

    sensor.begin();
    working = sensor.getID(id);

    if (!working) return;

    sensor.setTiming(false, 2, ms);
  };

  initializer();

  auto handler = [&](
    std::vector<uint8_t> *request,
    std::function<void (std::vector<uint8_t> response)> respond
  ) {
    initializer();

    if (!working) {
      respond({});
      return;
    }

    requestTime = millis();
    sensor.setPowerUp();

    responseCallback = [respond](std::vector<uint8_t> response) {
      respond(response);
    };
  };

  auto update = [&]() {
    if (!requestTime) return;

    unsigned long timeSinceRequest = millis() - requestTime;
    if (timeSinceRequest < 30000) return;
    requestTime = 0;

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

    responseCallback(response);
  };

  return {
    handler,
    update
  };
}
