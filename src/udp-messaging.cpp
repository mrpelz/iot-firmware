#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#define UDP_MAX_LENGTH 508

#define REQUEST_HEADER_LENGTH 2
#define INCOMING_MESSAGE_MAX_LENGTH UDP_MAX_LENGTH - REQUEST_HEADER_LENGTH

#define RESPONSE_HEADER_LENGTH 1
#define OUTGOING_MESSAGE_MAX_LENGTH UDP_MAX_LENGTH - RESPONSE_HEADER_LENGTH

#define EVENT_HEADER_LENGTH 2
#define EVENT_MESSAGE_MAX_LENGTH UDP_MAX_LENGTH - EVENT_HEADER_LENGTH

struct UDPIncomingMessagePayload {
  uint8_t requestId;
  uint8_t serviceId;
  uint8_t message[INCOMING_MESSAGE_MAX_LENGTH];
};

struct UDPIncomingMessage {
  IPAddress remoteAddress;
  uint16_t remotePort;
  UDPIncomingMessagePayload payload;
  uint16_t payloadLength;
};

struct UDPOutgoingMessagePayload {
  uint8_t requestId;
  uint8_t message[OUTGOING_MESSAGE_MAX_LENGTH];
};

struct UDPOutgoingMessage {
  IPAddress remoteAddress;
  uint16_t remotePort;
  UDPOutgoingMessagePayload payload;
  uint16_t payloadLength;
};

struct UDPEventMessagePayload {
  uint8_t requestId = 0;
  uint8_t eventId;
  uint8_t message[EVENT_MESSAGE_MAX_LENGTH];
};

struct UDPEventMessage {
  IPAddress remoteAddress;
  uint16_t remotePort;
  UDPEventMessagePayload payload;
  uint16_t payloadLength;
};

typedef std::function<void (
  UDPIncomingMessage request,
  UDPOutgoingMessage response
)> UDPServiceCallback;

struct UDPService {
  uint8_t id;
  UDPServiceCallback callback;
};

struct UDPMessagingConfig {
  uint16_t port;
};

struct UDPMessagingState {
  bool isListening = false;
  WiFiUDP udp;
  uint16_t port;
  uint8_t servicesPointer = 0;
  UDPService services[256];
  IPAddress lastPeerAddress;
  uint16_t lastPeerPort;
  std::function<void (const String &, const String &)> debugCallback =
    [](String key, String value) {};
};

class UDPMessaging {
  private:
    UDPMessagingState state;
    
  public:
    uint8_t EVENT_ID = 0;

    UDPMessaging(UDPMessagingConfig config) {
      state.port = config.port;
    }

    void addService(UDPService service) {
      if (state.servicesPointer >= 255) return;

      state.services[state.servicesPointer] = service;
      state.servicesPointer++;
    }

    void begin() {
      state.udp.begin(state.port);
      state.isListening = true;
    }

    bool isListening() {
      return state.isListening;
    }

    IPAddress lastPeerAddress() {
      return state.lastPeerAddress;
    }

    uint16_t lastPeerPort() {
      return state.lastPeerPort;
    }

    void setDebug(std::function<void (const String &, const String &)> callback) {
      state.debugCallback = callback;
    }

    void stop() {
      state.udp.stop();
      state.isListening = false;
    }

    void update() {
      if (!state.isListening) return;

      UDPIncomingMessage request;

      uint16_t udpLength = state.udp.parsePacket();
      if (!udpLength) return;

      request.remoteAddress = state.udp.remoteIP();
      request.remotePort = state.udp.remotePort();

      request.payloadLength = state.udp.read(
        (uint8_t*) &request.payload,
        UDP_MAX_LENGTH
      ) - REQUEST_HEADER_LENGTH;

      if (udpLength - request.payloadLength != REQUEST_HEADER_LENGTH) return;

      state.lastPeerAddress = request.remoteAddress;
      state.lastPeerPort = request.remotePort;

      state.debugCallback("udp.incoming-message.payload-length", String(request.payloadLength));
      state.debugCallback("udp.incoming-message.remote-address", request.remoteAddress.toString());
      state.debugCallback("udp.incoming-message.remote-port", String(request.remotePort));
      state.debugCallback("udp.incoming-message.request-id", String(request.payload.requestId));
      state.debugCallback("udp.incoming-message.service-id", String(request.payload.serviceId));

      for (uint16_t i = 0; i < request.payloadLength; i++) {
        state.debugCallback("udp.incoming-message.payload", String(request.payload.message[i]));
      }

      for (uint8_t i = 0; i < state.servicesPointer; i++) {
        if (state.services[i].id != request.payload.serviceId) continue;

        UDPOutgoingMessage response = {
          request.remoteAddress,
          request.remotePort,
          {
            request.payload.requestId
          }
        };

        state.services[i].callback(request, response);
      }
    }

    void write(UDPOutgoingMessage response) {
      state.debugCallback("udp.outgoing-message.payload-length", String(response.payloadLength));
      state.debugCallback("udp.outgoing-message.remote-address", response.remoteAddress.toString());
      state.debugCallback("udp.outgoing-message.remote-port", String(response.remotePort));
      state.debugCallback("udp.outgoing-message.request-id", String(response.payload.requestId));

      for (uint16_t i = 0; i < response.payloadLength; i++) {
        state.debugCallback("udp.outgoing-message.payload", String(response.payload.message[i]));
      }

      uint16_t packetLength = response.payloadLength + RESPONSE_HEADER_LENGTH;

      state.udp.beginPacket(response.remoteAddress, response.remotePort);
      state.udp.write((uint8_t*) &response.payload, packetLength);
      state.udp.endPacket();
    }

    void event(UDPEventMessage event) {
      state.debugCallback("udp.event-message.payload-length", String(event.payloadLength));
      state.debugCallback("udp.event-message.remote-address", event.remoteAddress.toString());
      state.debugCallback("udp.event-message.remote-port", String(event.remotePort));
      state.debugCallback("udp.event-message.request-id", String(event.payload.requestId));
      state.debugCallback("udp.event-message.event-id", String(event.payload.eventId));

      for (uint16_t i = 0; i < event.payloadLength; i++) {
        state.debugCallback("udp.event-message.payload", String(event.payload.message[i]));
      }

      uint16_t packetLength = event.payloadLength + EVENT_HEADER_LENGTH;

      state.udp.beginPacket(event.remoteAddress, event.remotePort);
      state.udp.write((uint8_t*) &event.payload, packetLength);
      state.udp.endPacket();
    }
};
