#ifndef _SERVICES_SERVICE_IDS
#define _SERVICES_SERVICE_IDS

namespace IotNode {
namespace Services {

enum ids {
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
  sds011,
  relais = 0xa0, // relais0 (start id)
  led = 0xb0, // led0 (start id)
  indicator = 0xc0, // indicator0 (start id)
  keepalive = 0xff
};

} // section namespace
} // project namespace

#endif
