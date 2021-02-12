#include "./handler.h"

RequestHandler makeMcp9808Handler() {
  bool working = false;
  Adafruit_MCP9808 sensor = Adafruit_MCP9808();

  auto initializer = [&]() {
    if (working) return;

    working = sensor.begin();
    if (!working) return;

    sensor.setResolution(3);
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

    std::vector<uint8_t> response;

    sensor.wake();
    float reading = sensor.readTempC();
    sensor.shutdown();

    response.insert(response.end(), &reading, &reading + sizeof(reading));

    respond(response);
  };

  return handler;
}
