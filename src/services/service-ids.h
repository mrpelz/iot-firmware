#ifndef _SERVICES_SERVICE_IDS
#define _SERVICES_SERVICE_IDS

namespace IotNode {
namespace Services {

enum serviceIds {
  _reserved_event,
  hello,
  systemInfo,
  async,
  mcp9808,
  bme280,
  tsl2561,
  sgp30,
  ccs811,
  veml6070,
  relais = 0xd0, // relais0 (start id)
  led = 0xe0, // led0 (start id)
  buzzer = 0xfd,
  blink = 0xfe,
  keepalive = 0xff
};

} // section namespace
} // project namespace

#endif
