#include "./vcc.h"

#ifdef IOT_NODE_VCC

ADC_MODE(ADC_VCC);

namespace IotNode::Utils::VCC
{
  bool read = false;

  uint16_t vcc = 0;

  void update()
  {
    if (read)
      return;

    uint16_t measurement = ESP.getVcc();

    if (!vcc)
    {
      vcc = measurement;
      return;
    }

    vcc = (vcc + measurement) / 2;
  }

  uint16_t get()
  {
    read = true;
    return vcc;
  }
}

#endif
