#ifdef IOT_NODE_VEML6070

#include "./main.h"

namespace IotNode::Services::Veml6070
{
  Utils::UDP::RespondCallback respondCallback = NULL;
  TaskHandle_t taskHandle = NULL;

  bool working = false;
  auto sensor = Sensor();

  void initializer(TwoWire *i2c)
  {
#ifdef IOT_NODE_LOGGING
    Utils::Log::debug("veml6070-service: initializing sensor");
#endif

    working = sensor.begin(i2c);
    if (!working)
    {
#ifdef IOT_NODE_LOGGING
      Utils::Log::debug("veml6070-service: sensor initialization failed");
#endif

      return;
    }

    sensor.sleep(true);

    xTaskCreatePinnedToCore(
        responseTask,
        "veml6070_handling",
        FREERTOS_STACK_SIZE,
        NULL,
        tskIDLE_PRIORITY,
        &taskHandle,
        CONFIG_ARDUINO_RUNNING_CORE);
  }

  void responseTask(void *parameter)
  {
    for (;;)
    {
      if (respondCallback == NULL)
      {
        vTaskSuspend(NULL);
        continue;
      }

      Utils::I2C::claim();

      sensor.sleep(false);
      sensor.waitForNext();

      auto reading = sensor.readUV();

      sensor.sleep(true);

      Utils::I2C::unclaim();

#ifdef IOT_NODE_LOGGING
      Utils::Log::debug(fmt::format("veml6070-service.reading: {}", reading));
#endif

      auto result = reinterpret_cast<unsigned char *>(&reading);

      Utils::UDP::Payload response;

      response.insert(
          response.end(),
          result,
          result + sizeof(reading));

#ifdef IOT_NODE_LOGGING
      Utils::Log::debug("veml6070-service: sending response");
#endif

      respondCallback(response);
      respondCallback = NULL;
    }
  }

  void handler(Utils::UDP::Payload *request, Utils::UDP::RespondCallback respond, Utils::UDP::Peer peer)
  {
#ifdef IOT_NODE_LOGGING
    Utils::Log::debug("veml6070-service: got request");
#endif

    if (!working)
    {
#ifdef IOT_NODE_LOGGING
      Utils::Log::debug("veml6070-service: sensor not working, sending null response");
#endif

      respond({});
      return;
    }

    respondCallback = respond;
    if (taskHandle != NULL)
      vTaskResume(taskHandle);
  }
}

#endif
