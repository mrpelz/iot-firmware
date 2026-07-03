#include "./safe-mode.h"
#include <pre.h>
#include <post.h>

namespace IotNode::Utils::SafeMode
{
  bool result = false;

  bool isSafeMode()
  {
    if (result)
      return result;

    pinMode(0, INPUT_PULLUP);

#ifdef IOT_NODE_ESP8266
    delay(1000);
#endif
#ifdef IOT_NODE_ESP32
    vTaskDelay(pdMS_TO_TICKS(1000));
#endif

    result = digitalRead(0) == LOW;
    pinMode(0, INPUT);

    return result;
  }
}
