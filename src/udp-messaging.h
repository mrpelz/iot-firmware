#ifndef _UDP_MESSAGING
#define _UDP_MESSAGING

#include <Arduino.h>
#include <ESPAsyncUDP.h>
#include "./services/service-ids.h"

#define UDP_MAX_LENGTH 508

#define MESSAGE_ID_LENGTH 1
#define SERVICE_ID_LENGTH 1
#define EVENT_ID_LENGTH 1

#define REQUEST_MIN_LENGTH MESSAGE_ID_LENGTH + SERVICE_ID_LENGTH
#define REQUEST_MAX_LENGTH UDP_MAX_LENGTH - REQUEST_MIN_LENGTH

#define RESPONSE_MAX_LENGTH UDP_MAX_LENGTH - MESSAGE_ID_LENGTH

#define EVENT_MAX_LENGTH UDP_MAX_LENGTH - MESSAGE_ID_LENGTH - EVENT_ID_LENGTH

struct UDPService {
  uint8_t serviceId;
  std::function<void (
    std::vector<uint8_t> *request,
    std::function<void (
      std::vector<uint8_t> response
    )> respond
  )> handler;
};

struct Peer {
  IPAddress remoteAddress;
  uint16_t remotePort;
};

struct UDPMessagingState {
  AsyncUDP udp;
  uint16_t port;
  Peer eventPeer;
  std::vector<UDPService *> services;
  std::function<void (const String &, const String &)> debugCallback =
    [](String key, String value) {};
};

class UDPMessaging {
  private:
    UDPMessagingState state;
    void handleRequest(AsyncUDPPacket *packet);
    
  public:
    UDPMessaging(uint16_t port);
    void addService(UDPService *service);
    void begin();
    void close();
    void event(uint8_t eventId, std::vector<uint8_t> event);
    void setDebug(std::function<void (const String &, const String &)> callback);
};

#endif
