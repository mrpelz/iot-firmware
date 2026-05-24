#include "./main.h"

#ifdef IOT_NODE_VCC

namespace IotNode
{
  namespace Events
  {

    namespace VCC
    {
#ifdef IOT_NODE_ESP_NOW_NODE
      ChangeCallback makeEspNowEvent()
      {
        auto handler = [](uint16_t vcc)
        {
          ::std::vector<uint8_t> response = {
              ids::vcc,
              0};

          auto value = reinterpret_cast<uint8_t *>(&(vcc));

          response.insert(
              response.end(),
              value,
              value + sizeof(vcc));

          Utils::EspNowNode::send(response);
        };

        return handler;
      }
#endif
    }

  } // section namespace
} // project namespace

#endif
