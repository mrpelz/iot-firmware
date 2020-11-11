#ifndef _SERVICE_IDS
#define _SERVICE_IDS

enum serviceIds {
  _reserved_event,
  hello,
  systemInfo,
  async,
  relais0 = 0xd0,
  led0 = 0xe0,
  keepalive = 0xff
};

#endif
