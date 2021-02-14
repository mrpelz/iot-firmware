#include "./handler.h"

RequestHandler makeMcp9808Handler() {
  bool working = false;
  Adafruit_MCP9808 sensor = Adafruit_MCP9808();

  auto initializer = [&]() {
    if (working) return;

    debug("mcp9808-service", "initializing sensor");

    working = sensor.begin();
    if (!working) {
      debug("mcp9808-service", "sensor initialization failed");
      return;
    }

    sensor.setResolution(3);
  };

  auto handler = [&](
    std::vector<uint8_t> *request,
    std::function<void (std::vector<uint8_t> response)> respond
  ) {
    debug("mcp9808-service", "got request");

    initializer();

    if (!working) {
      debug("mcp9808-service", "sensor not working, sending null response");

      respond({});
      return;
    }

    std::vector<uint8_t> response;

    sensor.wake();
    float reading = sensor.readTempC();
    sensor.shutdown();

    response.insert(response.end(), &reading, &reading + sizeof(reading));

    debug("mcp9808-service", "sending response");

    respond(response);
  };

  return handler;
}
