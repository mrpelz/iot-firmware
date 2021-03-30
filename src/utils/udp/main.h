#ifndef _UTILS_UDP_MAIN
#define _UTILS_UDP_MAIN

#include <Arduino.h>

#ifdef ARDUINO_ARCH_ESP8266
  #include <ESPAsyncUDP.h>  
#endif
#ifdef ARDUINO_ARCH_ESP32
  #include <AsyncUDP.h>
#endif

#include "../../events/event-ids.h"
#include "../../services/service-ids.h"
#include "../log.h"

#define UDP_MAX_LENGTH 508

#define MESSAGE_ID_LENGTH 1
#define SERVICE_ID_LENGTH 1
#define EVENT_ID_LENGTH 1

#define PEER_SET_MIN_LENGTH 2

#define REQUEST_MIN_LENGTH MESSAGE_ID_LENGTH + SERVICE_ID_LENGTH
#define REQUEST_MAX_LENGTH UDP_MAX_LENGTH - REQUEST_MIN_LENGTH

#define RESPONSE_MAX_LENGTH UDP_MAX_LENGTH - MESSAGE_ID_LENGTH

#define EVENT_MAX_LENGTH UDP_MAX_LENGTH - MESSAGE_ID_LENGTH - EVENT_ID_LENGTH

namespace IotNode {
namespace Utils {

namespace UDP {
  typedef std::vector<uint8_t> Payload;

  typedef std::function<void (
    Payload response
  )> RespondCallback;

  typedef std::function<void (
    Payload *request,
    RespondCallback respond
  )> RequestHandler;

  struct Service {
    uint8_t serviceId;
    RequestHandler handler;
  };

  struct Peer {
    IPAddress ip;
    uint16_t port;
  };

  struct State {
    bool isListening = false;
    AsyncUDP udp;
    uint16_t port;
    Peer eventPeer;
    Peer fallbackPeer;
    uint8_t eventPeerPriority = 0;
    std::vector<Service *> services;
    Log::Callback debugCallback = [](String key, String value) {};
  };

  class Class {
    private:
      State state;
      void handleRequest(AsyncUDPPacket *packet);
      
    public:
      Class(uint16_t port);
      void addService(Service *service);
      void begin();
      void close();
      void event(uint8_t eventId, Payload event);
      bool hasEventPeer();
      bool isListening();
      void setDebug(Log::Callback callback);
  };
}

} // section namespace
} // project namespace

#endif
