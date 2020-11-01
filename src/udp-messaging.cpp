#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#define UDP_MAX_LENGTH 508

#define REQUEST_HEADER_LENGTH 2
#define REQUEST_MAX_LENGTH UDP_MAX_LENGTH - REQUEST_HEADER_LENGTH

#define RESPONSE_HEADER_LENGTH 1
#define RESPONSE_MAX_LENGTH UDP_MAX_LENGTH - RESPONSE_HEADER_LENGTH

#define EVENT_HEADER_LENGTH 2
#define EVENT_MAX_LENGTH UDP_MAX_LENGTH - EVENT_HEADER_LENGTH

struct Peer {
  IPAddress remoteAddress;
  uint16_t remotePort;
};

struct EventPayload {
  uint8_t requestId = 0;
  uint8_t eventId;
};

struct Event {
  uint16_t *length;
  EventPayload *payload;
};

struct ResponsePayload {
  uint8_t requestId;
};

struct Response {
  uint16_t *length;
  ResponsePayload *payload;
};

struct RequestPayload {
  uint8_t requestId;
  uint8_t serviceId;
};

struct Request {
  uint16_t *length;
  RequestPayload *payload;
};

struct UDPServiceState {
  uint8_t serviceId;
  Request *request;
  Response *response;
  Peer *peer;
  std::function<void (const String &, const String &)> debugCallback =
    [](String key, String value) {};
};

class UDPService {
  private:
    UDPServiceState state;

  public:
    UDPService(uint8_t serviceId) {
      state.serviceId = serviceId;
    }

    uint8_t *serviceId() {
      return &state.serviceId;
    }

    Peer *getPeer() {
      return state.peer;
    }

    Request *getRequest() {
      if (state.request == nullptr) return nullptr;

      Request *request = state.request;
      state.request = nullptr;

      return request;
    }

    Response *getResponse() {
      if (state.response == nullptr) return nullptr;

      Response *response = state.response;
      state.response = nullptr;

      return response;
    }

    void ingestRequest(Request *request, Peer *peer) {
      delete state.request;
      state.request = request;

      state.peer = peer;
    }

    void ingestResponsePayload(ResponsePayload *responsePayload, uint16_t length) {
      uint16_t packetLength = length + RESPONSE_HEADER_LENGTH;

      responsePayload->requestId = state.request->payload->requestId;

      delete state.response;
      state.response = new Response({
        &packetLength,
        responsePayload
      });
    }

    void setDebug(std::function<void (const String &, const String &)> callback) {
      state.debugCallback = callback;
    }
};

struct UDPMessagingState {
  bool isListening;
  WiFiUDP udp;
  uint16_t port;
  std::vector<UDPService *> services;
  Event *event;
  Peer *peer;
  std::function<void (const String &, const String &)> debugCallback =
    [](String key, String value) {};
};

class UDPMessaging {
  private:
    UDPMessagingState state;

    void handleEvent() {
      if (state.event == nullptr) return;

      Event *event = state.event;
      state.event = nullptr;

      state.udp.beginPacket(state.peer->remoteAddress, state.peer->remotePort);
      state.udp.write((uint8_t *)event->payload, *event->length);
      state.udp.endPacket();
    }

    void handleRequests() {
      uint16_t length = state.udp.parsePacket();
      if (!length) return;

      delete state.peer;
      state.peer = new Peer({
        state.udp.remoteIP(),
        state.udp.remotePort()
      });

      RequestPayload *payload = (RequestPayload *)malloc(length);

      state.udp.read((uint8_t *)payload, length);

      auto request = new Request({
        &length,
        payload
      });

      std::for_each(
        std::begin(state.services),
        std::end(state.services),
        [&](UDPService * const service) {
          if (*(service->serviceId()) != payload->serviceId) return;

          service->ingestRequest(request, state.peer);
        }
      );
    }

    void handleResponses() {
      std::for_each(
        std::begin(state.services),
        std::end(state.services),
        [&](UDPService * const service) {
          Response *response = service->getResponse();

          if (!response) return;

          state.udp.beginPacket(state.peer->remoteAddress, state.peer->remotePort);
          state.udp.write((uint8_t *)response->payload, *response->length);
          state.udp.endPacket();
        }
      );
    }
    
  public:
    UDPMessaging(uint16_t port, std::vector<UDPService *> services) {
      state.isListening = false;

      state.port = port;
      state.services = services;
    }

    void begin() {
      state.isListening = true;

      state.debugCallback("event", "udp.listening");
      state.debugCallback("udp.listening.port", String(state.port));

      state.udp.begin(state.port);
    }

    void ingestEventPayload(EventPayload *eventPayload, uint16_t length) {
      uint16_t packetLength = length + EVENT_HEADER_LENGTH;

      delete state.event;
      state.event = new Event({
        &packetLength,
        eventPayload
      });
    }

    void setDebug(std::function<void (const String &, const String &)> callback) {
      state.debugCallback = callback;
    }

    void stop() {
      state.isListening = false;
      state.udp.stop();
    }

    void update() {
      if (!state.isListening) return;

      handleEvent();
      handleResponses();
      handleRequests();
    }
};
