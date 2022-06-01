#include "./main.h"

namespace IotNode {
namespace Utils {

namespace UDP {
  Class::Class(uint16_t port) {
    state.port = port;

    state.udp.onPacket([&](AsyncUDPPacket packet) {
      handleRequest(&packet);
    });
  }

  void Class::addService(Service *service) {
    state.services.insert(state.services.end(), service);
  }

  void Class::begin() {
    if (state.isListening) return;

    #ifdef IOT_NODE_LOGGING
      Log::debug("event", "udp.listening");
      Log::debug("udp.listening.port", String(state.port));
    #endif

    state.isListening = true;
    state.udp.listen(state.port);
  }

  void Class::close() {
    if (!state.isListening) return;

    #ifdef IOT_NODE_LOGGING
      Log::debug("event", "udp.listening");
      Log::debug("udp.listening", "close");
    #endif

    removeEventPeer();

    state.isListening = false;
    state.udp.close();
  }

  void Class::event(uint8_t eventId, uint8_t eventIndex, Payload event) {
    if (!state.eventPeer.port) return;

    Payload outgoing = {
      Services::ids::_reserved_event,
      eventId,
      eventIndex
    };

    uint8_t *eventStart = event.data();
    uint8_t *eventEnd = eventStart + event.size();
    outgoing.insert(outgoing.end(), eventStart, eventEnd);

    #ifdef IOT_NODE_LOGGING
      Log::debug("udp.event.length", String(outgoing.size()));
      Log::debug("udp.event.remote-ip", state.eventPeer.ip.toString());
      Log::debug("udp.event.remote-port", String(state.eventPeer.port));
      Log::debug("udp.event.event-id", String(eventId, HEX));
      Log::debug("udp.event.event-index", String(eventIndex, HEX));
      Log::debug("udp.event.message-length", String(event.size()));
    #endif

    state.udp.writeTo(
      outgoing.data(),
      outgoing.size(),
      state.eventPeer.ip,
      state.eventPeer.port
    );
  }

  void Class::handleRequest(AsyncUDPPacket *packet) {
    #ifdef IOT_NODE_LOGGING
      Log::debug("event", "udp.request");
    #endif

    size_t length = packet->length();

    if (length < REQUEST_MIN_LENGTH) {
      packet->flush();

      return;
    }

    Peer peer = {
      packet->remoteIP(),
      packet->remotePort()
    };

    auto validPeer = peer.port && (peer.ip[0] + peer.ip[1] + peer.ip[2] + peer.ip[3]);
    if (validPeer) {
      state.fallbackPeer = peer;
    } else if (!state.fallbackPeer.port) {
      #ifdef IOT_NODE_LOGGING
        Log::debug("event", "udp.request.no-usable-peer");
      #endif

      return;
    } else {
      peer = state.fallbackPeer;

      #ifdef IOT_NODE_LOGGING
        Log::debug("event", "udp.request.use-fallback-peer");
      #endif
    }

    auto payload = packet->data();

    auto messageId = ((uint8_t *)payload)[0];
    auto messageVersion = ((uint8_t *)payload)[1];
    auto serviceId = ((uint8_t *)payload)[2];
    auto serviceIndex = ((uint8_t *)payload)[3];

    uint8_t *messageStart = payload + MESSAGE_ID_LENGTH + MESSAGE_VERSION_LENGTH + SERVICE_ID_LENGTH + SERVICE_INDEX_LENGTH;
    uint8_t *messageEnd = payload + length;
    
    Payload message;
    message.insert(message.end(), messageStart, messageEnd);

    packet->flush();

    #ifdef IOT_NODE_LOGGING
      Log::debug("udp.request.length", String(length));
      Log::debug("udp.request.remote-ip", peer.ip.toString());
      Log::debug("udp.request.remote-port", String(peer.port));
      Log::debug("udp.request.message-id", String(messageId, HEX));
      Log::debug("udp.request.message-version", String(messageVersion, HEX));
      Log::debug("udp.request.service-id", String(serviceId, HEX));
      Log::debug("udp.request.service-index", String(serviceIndex, HEX));
      Log::debug("udp.request.message-length", String(message.size()));
    #endif

    if (messageVersion != MESSAGE_VERSION) {
      #ifdef IOT_NODE_LOGGING
        Log::debug("event", "udp.request.no-matching-version");
      #endif
      
      return;
    }

    bool match = false;
    std::for_each(
      std::begin(state.services),
      std::end(state.services),
      [&](Service *service) {
        if (service->serviceId != serviceId) return;
        if (service->serviceIndex != serviceIndex) return;

        match = true;

        service->handler(
          &message,
          [this, messageId, serviceId, peer](Payload response) {
            Payload outgoing = {
              messageId
            };

            uint8_t *responseStart = response.data();
            uint8_t *responseEnd = responseStart + response.size();
            outgoing.insert(outgoing.end(), responseStart, responseEnd);

            #ifdef IOT_NODE_LOGGING
              Log::debug("udp.response.length", String(outgoing.size()));
              Log::debug("udp.response.remote-ip", peer.ip.toString());
              Log::debug("udp.response.remote-port", String(peer.port));
              Log::debug("udp.response.message-id", String(messageId, HEX));
              Log::debug("udp.response.service-id", String(serviceId, HEX));
              Log::debug("udp.response.message-length", String(response.size()));
            #endif

            state.udp.writeTo(
              outgoing.data(),
              outgoing.size(),
              peer.ip,
              peer.port
            );
          },
          peer
        );
      }
    );

    if (!match) {
      #ifdef IOT_NODE_LOGGING
        Log::debug("event", "udp.request.no-matching-service");
      #endif
    }
  }

  bool Class::hasEventPeer() {
    return !!state.eventPeer.port;
  }

  bool Class::isListening() {
    return !!state.isListening;
  }

  void Class::removeEventPeer() {
    state.eventPeer.port = 0;
  }

  void Class::setEventPeer(Peer peer) {
    state.eventPeer = peer;
  }
}

} // section namespace
} // project namespace
