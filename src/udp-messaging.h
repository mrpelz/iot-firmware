#ifndef _UDP_MESSAGING
#define _UDP_MESSAGING

#include <Arduino.h>

#ifdef ARDUINO_ARCH_ESP8266
  #include <ESPAsyncUDP.h>  
#endif
#ifdef ARDUINO_ARCH_ESP32
  #include <AsyncUDP.h>
#endif

#include "./events/event-ids.h"
#include "./services/service-ids.h"
#include "./logging.h"

#define UDP_MAX_LENGTH 508

#define MESSAGE_ID_LENGTH 1
#define SERVICE_ID_LENGTH 1
#define EVENT_ID_LENGTH 1

#define REQUEST_MIN_LENGTH MESSAGE_ID_LENGTH + SERVICE_ID_LENGTH
#define REQUEST_MAX_LENGTH UDP_MAX_LENGTH - REQUEST_MIN_LENGTH

#define RESPONSE_MAX_LENGTH UDP_MAX_LENGTH - MESSAGE_ID_LENGTH

#define EVENT_MAX_LENGTH UDP_MAX_LENGTH - MESSAGE_ID_LENGTH - EVENT_ID_LENGTH

typedef std::function<void (
  std::vector<uint8_t> response
)> RespondCallback;

typedef std::function<void (
  std::vector<uint8_t> *request,
  RespondCallback respond
)> RequestHandler;

struct UDPService {
  uint8_t serviceId;
  RequestHandler handler;
};

struct Peer {
  IPAddress ip;
  uint16_t port;
};

struct UDPMessagingState {
  bool isListening = false;
  AsyncUDP udp;
  uint16_t port;
  Peer eventPeer;
  std::vector<UDPService *> services;
  LoggingCallback debugCallback = [](String key, String value) {};
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
    bool hasEventPeer();
    bool isListening();
    void setDebug(LoggingCallback callback);
};

#endif
