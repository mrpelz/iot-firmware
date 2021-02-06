#ifndef _SERVICE_IDS
#define _SERVICE_IDS

enum serviceIds {
  _reserved_event,
  hello,
  systemInfo,
  async,
  mcp9808,
  relais = 0xd0, // relais0 (start id)
  led = 0xe0, // led0 (start id)
  keepalive = 0xff
};

#endif
