#ifndef _SERVICE_IDS
#define _SERVICE_IDS

namespace IotNode {

enum serviceIds {
  _reserved_event,
  hello,
  systemInfo,
  async,
  mcp9808,
  bme280,
  tsl2561,
  sgp30,
  relais = 0xd0, // relais0 (start id)
  led = 0xe0, // led0 (start id)
  keepalive = 0xff
};

} // project namespace

#endif
