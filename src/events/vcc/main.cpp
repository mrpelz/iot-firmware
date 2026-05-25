#include "./main.h"

#ifdef IOT_NODE_VCC

namespace IotNode::Events::VCC
{
#ifdef IOT_NODE_ESP_NOW_NODE
  ChangeCallback makeEspNowEvent()
  {
    auto handler = [](unsigned short vcc)
    {
      ::std::vector<unsigned char> response = {
          ids::vcc,
          0};

      auto value = reinterpret_cast<unsigned char *>(&(vcc));

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

#endif
