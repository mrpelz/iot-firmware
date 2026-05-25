#include "./vcc.h"

#ifdef IOT_NODE_VCC

ADC_MODE(ADC_VCC);

namespace IotNode::Utils::VCC
{
  bool read = false;

  unsigned short vcc = 0;

  void update()
  {
    if (read)
      return;

    unsigned short measurement = ESP.getVcc();

    if (!vcc)
    {
      vcc = measurement;
      return;
    }

    vcc = (vcc + measurement) / 2;
  }

  unsigned short get()
  {
    read = true;
    return vcc;
  }
}

#endif
