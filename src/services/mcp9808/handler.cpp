#include "./handler.h"

RequestHandler makeMcp9808Handler() {
  auto working = std::make_shared<bool>(false);
  auto sensor = std::make_shared<Adafruit_MCP9808>(Adafruit_MCP9808());

  auto initializer = [&, working, sensor]() {
    if (*working) return;

    debug("mcp9808-service", "initializing sensor");

    *working = sensor->begin();
    if (!*working) {
      debug("mcp9808-service", "sensor initialization failed");
      return;
    }

    sensor->setResolution(3);
  };

  auto handler = [&, working, sensor](
    std::vector<uint8_t> *request,
    std::function<void (std::vector<uint8_t> response)> respond
  ) {
    debug("mcp9808-service", "got request");

    initializer();

    if (!*working) {
      debug("mcp9808-service", "sensor not working, sending null response");

      respond({});
      return;
    }

    std::vector<uint8_t> response;

    sensor->wake();
    float reading = sensor->readTempC();
    sensor->shutdown();

    response.insert(response.end(), &reading, &reading + sizeof(reading));

    debug("mcp9808-service", "sending response");

    respond(response);
  };

  return handler;
}
