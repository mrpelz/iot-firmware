#ifndef _SERVICE_IDS
#define _SERVICE_IDS

enum serviceIds {
  _reserved_event,
  hello,
  systemInfo,
  async,
  keepalive = 0xFF
};

#endif
