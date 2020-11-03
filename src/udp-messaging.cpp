#include <Arduino.h>
#include <ESPAsyncUDP.h>

#define UDP_MAX_LENGTH 508

#define MESSAGE_ID_LENGTH 1
#define SERVICE_ID_LENGTH 1
#define EVENT_ID_LENGTH 1

#define REQUEST_MIN_LENGTH MESSAGE_ID_LENGTH + SERVICE_ID_LENGTH
#define REQUEST_MAX_LENGTH UDP_MAX_LENGTH - REQUEST_MIN_LENGTH

#define RESPONSE_MAX_LENGTH UDP_MAX_LENGTH - MESSAGE_ID_LENGTH

#define EVENT_ID_FIELD 0x00
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
  Peer peer;
  std::vector<UDPService *> services;
  std::function<void (const String &, const String &)> debugCallback =
    [](String key, String value) {};
};

class UDPMessaging {
  private:
    UDPMessagingState state;

    void handleRequest(AsyncUDPPacket *packet) {
      state.debugCallback("event", "udp.request");

      size_t length = packet->length();

      if (length < REQUEST_MIN_LENGTH) {
        packet->flush();
        return;
      }

      state.peer = {
        packet->remoteIP(),
        packet->remotePort()
      };

      auto payload = packet->data();
      uint8_t messageId = *payload;
      uint8_t serviceId = *(payload + MESSAGE_ID_LENGTH);
      uint8_t *messageStart = payload + MESSAGE_ID_LENGTH + SERVICE_ID_LENGTH;
      uint8_t *messageEnd = payload + length;
      
      std::vector<uint8_t> message;
      message.insert(message.end(), messageStart, messageEnd);

      state.debugCallback("udp.request.length", String(length));
      state.debugCallback("udp.request.remote-ip", packet->remoteIP().toString());
      state.debugCallback("udp.request.remote-port", String(packet->remotePort()));
      state.debugCallback("udp.request.message-id", String(messageId, HEX));
      state.debugCallback("udp.request.service-id", String(serviceId, HEX));
      state.debugCallback("udp.request.message-length", String(message.size()));

      auto respond = [&](std::vector<uint8_t> response) {
        std::vector<uint8_t> outgoing = {
          messageId
        };

        uint8_t *responseStart = response.data();
        uint8_t *responseEnd = responseStart + response.size();
        outgoing.insert(outgoing.end(), responseStart, responseEnd);

        state.debugCallback("udp.response.length", String(outgoing.size()));
        state.debugCallback("udp.response.message-id", String(messageId, HEX));
        state.debugCallback("udp.response.message-length", String(response.size()));

        packet->write(outgoing.data(), outgoing.size());
      };

      bool match = false;
      std::for_each(
        std::begin(state.services),
        std::end(state.services),
        [&](UDPService *service) {
          if (service->serviceId != serviceId) return;

          match = true;
          service->handler(&message, respond);
        }
      );

      if (!match) {
        state.debugCallback("event", "udp.request.no-matching-service");
        packet->flush();
      }
    }
    
  public:
    UDPMessaging(uint16_t port) {
      state.port = port;

      state.udp.onPacket([&](AsyncUDPPacket packet) {
        handleRequest(&packet);
      });
    }

    void addService(UDPService *service) {
      state.services.insert(state.services.end(), service);
    }

    void begin() {
      state.debugCallback("event", "udp.listening");
      state.debugCallback("udp.listening.port", String(state.port));

      state.udp.listen(state.port);
    }

    void close() {
      state.debugCallback("event", "udp.listening");
      state.debugCallback("udp.listening", "close");

      state.udp.close();
    }

    void event(uint8_t eventId, std::vector<uint8_t> event) {
      if (!state.peer.remotePort) return;

      std::vector<uint8_t> outgoing = {
          EVENT_ID_FIELD,
          eventId
        };

        uint8_t *eventStart = event.data();
        uint8_t *eventEnd = eventStart + event.size();
        outgoing.insert(outgoing.end(), eventStart, eventEnd);

        state.debugCallback("udp.event.length", String(outgoing.size()));
        state.debugCallback("udp.event.remote-ip", state.peer.remoteAddress.toString());
        state.debugCallback("udp.event.remote-port", String(state.peer.remotePort));
        state.debugCallback("udp.event.event-id", String(eventId, HEX));
        state.debugCallback("udp.event.message-length", String(event.size()));

        state.udp.writeTo(
          outgoing.data(),
          outgoing.size(),
          state.peer.remoteAddress,
          state.peer.remotePort
        );
    }

    void setDebug(std::function<void (const String &, const String &)> callback) {
      state.debugCallback = callback;
    }
};
