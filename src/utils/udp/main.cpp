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

    state.debugCallback("event", "udp.listening");
    state.debugCallback("udp.listening.port", String(state.port));

    state.isListening = true;
    state.udp.listen(state.port);
  }

  void Class::close() {
    if (!state.isListening) return;

    state.debugCallback("event", "udp.listening");
    state.debugCallback("udp.listening", "close");

    state.isListening = false;
    state.udp.close();
  }

  void Class::event(uint8_t eventId, Payload event) {
    if (!state.eventPeer.port) return;

    Payload outgoing = {
      Services::ids::_reserved_event,
      eventId
    };

    uint8_t *eventStart = event.data();
    uint8_t *eventEnd = eventStart + event.size();
    outgoing.insert(outgoing.end(), eventStart, eventEnd);

    state.debugCallback("udp.event.length", String(outgoing.size()));
    state.debugCallback("udp.event.remote-ip", state.eventPeer.ip.toString());
    state.debugCallback("udp.event.remote-port", String(state.eventPeer.port));
    state.debugCallback("udp.event.event-id", String(eventId, HEX));
    state.debugCallback("udp.event.message-length", String(event.size()));

    state.udp.writeTo(
      outgoing.data(),
      outgoing.size(),
      state.eventPeer.ip,
      state.eventPeer.port
    );
  }

  void Class::handleRequest(AsyncUDPPacket *packet) {
    state.debugCallback("event", "udp.request");

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
    }

    auto payload = packet->data();
    auto messageId = ((uint8_t *)payload)[0];
    auto serviceId = ((uint8_t *)payload)[1];
    uint8_t *messageStart = payload + MESSAGE_ID_LENGTH + SERVICE_ID_LENGTH;
    uint8_t *messageEnd = payload + length;
    
    Payload message;
    message.insert(message.end(), messageStart, messageEnd);

    packet->flush();

    state.debugCallback("udp.request.length", String(length));
    state.debugCallback("udp.request.remote-ip", peer.ip.toString());
    state.debugCallback("udp.request.remote-port", String(peer.port));
    state.debugCallback("udp.request.message-id", String(messageId, HEX));
    state.debugCallback("udp.request.service-id", String(serviceId, HEX));
    state.debugCallback("udp.request.message-length", String(message.size()));

    if (serviceId == Services::ids::_reserved_event) {
      Payload peerAckOutgoing = {
        messageId
      };

      if (message.size() < PEER_SET_MIN_LENGTH) {
        state.debugCallback("udp.request.set-event-peer", "missing parameters");
      } else {
        auto setPeer = message.at(0);
        auto setPeerPriority = message.at(1);

        if (setPeerPriority > 0 && setPeerPriority >= state.eventPeerPriority) {
          if (setPeer == 0) {
            state.eventPeer.port = 0;
            state.eventPeerPriority = 0;

            peerAckOutgoing.insert(peerAckOutgoing.end(), 0x00);

            state.debugCallback("udp.request.set-event-peer", "clear");
          } else if (validPeer) {
            state.eventPeer = peer;
            state.eventPeerPriority = setPeerPriority;

            peerAckOutgoing.insert(peerAckOutgoing.end(), 0x01);

            state.debugCallback("udp.request.set-event-peer", "set");
            state.debugCallback("udp.request.set-event-peer.ip", peer.ip.toString());
            state.debugCallback("udp.request.set-event-peer.port", String(peer.port));
            state.debugCallback("udp.request.set-event-peer.priority", String(setPeerPriority));
          }
        } else {
          peerAckOutgoing.insert(peerAckOutgoing.end(), 0x03);

          state.debugCallback("udp.request.set-event-peer", "rejected");
        }
      }


      state.udp.writeTo(
        peerAckOutgoing.data(),
        peerAckOutgoing.size(),
        peer.ip,
        peer.port
      );

      return;
    }

    auto responsePeer = validPeer ? peer : state.fallbackPeer;
    if (!responsePeer.port) {
      state.debugCallback("event", "udp.request.no-usable-peer");
      return;
    }

    if (!validPeer) {
      state.debugCallback("event", "udp.request.use-fallback-peer");
    }

    bool match = false;
    std::for_each(
      std::begin(state.services),
      std::end(state.services),
      [&](Service *service) {
        if (service->serviceId != serviceId) return;
        match = true;

        service->handler(
          &message,
          [this, messageId, serviceId, responsePeer](Payload response) {
            Payload outgoing = {
              messageId
            };

            uint8_t *responseStart = response.data();
            uint8_t *responseEnd = responseStart + response.size();
            outgoing.insert(outgoing.end(), responseStart, responseEnd);

            state.debugCallback("udp.response.length", String(outgoing.size()));
            state.debugCallback("udp.response.remote-ip", responsePeer.ip.toString());
            state.debugCallback("udp.response.remote-port", String(responsePeer.port));
            state.debugCallback("udp.response.message-id", String(messageId, HEX));
            state.debugCallback("udp.response.service-id", String(serviceId, HEX));
            state.debugCallback("udp.response.message-length", String(response.size()));

            state.udp.writeTo(
              outgoing.data(),
              outgoing.size(),
              responsePeer.ip,
              responsePeer.port
            );
          }
        );
      }
    );

    if (!match) {
      state.debugCallback("event", "udp.request.no-matching-service");
    }
  }

  bool Class::hasEventPeer() {
    return !!state.eventPeer.port;
  }

  bool Class::isListening() {
    return !!state.isListening;
  }

  void Class::setDebug(Log::Callback callback) {
    state.debugCallback = callback;
  }
}

} // section namespace
} // project namespace
