#include "./setup.h"

#ifdef IOT_NODE_INPUT

namespace IotNode
{
  namespace Events
  {

    namespace Input
    {
#if defined(IOT_NODE_BOARD_ROOM_SENSOR) || defined(IOT_NODE_BOARD_TEST_DEVICE)
      Class input0(39, true, 50, 0);
#elif defined(IOT_NODE_BOARD_ESP_NOW_TEST_WINDOW_SENSOR)
      Class input0(12, true, 50, 0);
      Class input1(13, true, 50, 0);
      Class input2(14, true, 50, 0);
#elif defined(IOT_NODE_BOARD_MOTION_SENSOR)
      Class input0(1, true, 50, 0);
#elif defined(IOT_NODE_BOARD_SHELLY1) && defined(IOT_NODE_INPUT_NOISE_GATE)
      Class input0(3, true, 50, 3000);
#elif defined(IOT_NODE_BOARD_SHELLY1)
      Class input0(3, true, 50, 0);
#endif

      void update()
      {
#if defined(IOT_NODE_BOARD_ROOM_SENSOR) || defined(IOT_NODE_BOARD_TEST_DEVICE) || defined(IOT_NODE_BOARD_MOTION_SENSOR) || defined(IOT_NODE_BOARD_SHELLY1)
        input0.update();
#elif defined(IOT_NODE_BOARD_ESP_NOW_TEST_WINDOW_SENSOR)
        input0.update();
        input1.update();
        input2.update();
#endif
      }

#ifdef IOT_NODE_ESP32
      void task(void *parameter)
      {
        for (;;)
        {
          update();
          vTaskDelay(IOT_NODE_MUTLITASKING_DELAY / portTICK_PERIOD_MS);
        }
      }
#endif

      void setup()
      {
#if defined(IOT_NODE_BOARD_ROOM_SENSOR) || defined(IOT_NODE_BOARD_TEST_DEVICE) || defined(IOT_NODE_BOARD_MOTION_SENSOR) || defined(IOT_NODE_BOARD_SHELLY1)
        auto event0 = makeEvent(&Utils::UDP::instance, 0);
        input0.setChangeCallback(event0);
        input0.start();
#elif defined(IOT_NODE_BOARD_ESP_NOW_TEST_WINDOW_SENSOR)
        auto event0 = makeEvent(&Utils::UDP::instance, 0);
        input0.setChangeCallback(event0);
        input0.start();

        auto event1 = makeEvent(&Utils::UDP::instance, 1);
        input1.setChangeCallback(event1);
        input1.start();

        auto event2 = makeEvent(&Utils::UDP::instance, 2);
        input2.setChangeCallback(event2);
        input2.start();
#endif

#ifdef IOT_NODE_ESP32
        xTaskCreatePinnedToCore(
            task,
            "input_maintenance",
            2048,
            NULL,
            3,
            NULL,
            CONFIG_ARDUINO_RUNNING_CORE);
#endif
      }

#ifdef IOT_NODE_ESP_NOW_NODE
      void setupEspNow()
      {
#if defined(IOT_NODE_BOARD_ROOM_SENSOR) || defined(IOT_NODE_BOARD_TEST_DEVICE) || defined(IOT_NODE_BOARD_MOTION_SENSOR) || defined(IOT_NODE_BOARD_SHELLY1)
        auto event0 = makeEspNowEvent(0);
        input0.setChangeCallback(event0);
        input0.start();
#elif defined(IOT_NODE_BOARD_ESP_NOW_TEST_WINDOW_SENSOR)
        auto event0 = makeEspNowEvent(0);
        auto event1 = makeEspNowEvent(1);
        auto event2 = makeEspNowEvent(2);

        input0.setChangeCallback(event0);
        input1.setChangeCallback(event1);
        input2.setChangeCallback(event2);

        input0.start();
        input1.start();
        input2.start();

        input0.update(true);
        input1.update(true);
        input2.update(true);
#endif

#ifdef IOT_NODE_ESP32
        xTaskCreatePinnedToCore(
            task,
            "input_maintenance",
            2048,
            NULL,
            3,
            NULL,
            CONFIG_ARDUINO_RUNNING_CORE);
#endif
      }
#endif
    }

  } // section namespace
} // project namespace

#endif
