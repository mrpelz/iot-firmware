#include "./udp-messaging.h"

UDPMessaging::UDPMessaging(uint16_t port) {
  state.port = port;

  state.udp.onPacket([&](AsyncUDPPacket packet) {
    handleRequest(&packet);
  });
}

void UDPMessaging::addService(UDPService *service) {
  state.services.insert(state.services.end(), service);
}

void UDPMessaging::begin() {
  if (state.isListening) return;

  state.debugCallback("event", "udp.listening");
  state.debugCallback("udp.listening.port", String(state.port));

  state.isListening = true;
  state.udp.listen(state.port);
}

void UDPMessaging::close() {
  if (!state.isListening) return;

  state.debugCallback("event", "udp.listening");
  state.debugCallback("udp.listening", "close");

  state.isListening = false;
  state.udp.close();
}

void UDPMessaging::event(uint8_t eventId, std::vector<uint8_t> event) {
  if (!state.eventPeer.port) return;

  std::vector<uint8_t> outgoing = {
    serviceIds::_reserved_event,
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

void UDPMessaging::handleRequest(AsyncUDPPacket *packet) {
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

  auto payload = packet->data();
  uint8_t messageId = *payload;
  uint8_t serviceId = *(payload + MESSAGE_ID_LENGTH);
  uint8_t *messageStart = payload + MESSAGE_ID_LENGTH + SERVICE_ID_LENGTH;
  uint8_t *messageEnd = payload + length;
  
  std::vector<uint8_t> message;
  message.insert(message.end(), messageStart, messageEnd);

  packet->flush();

  state.debugCallback("udp.request.length", String(length));
  state.debugCallback("udp.request.remote-ip", peer.ip.toString());
  state.debugCallback("udp.request.remote-port", String(peer.port));
  state.debugCallback("udp.request.message-id", String(messageId, HEX));
  state.debugCallback("udp.request.service-id", String(serviceId, HEX));
  state.debugCallback("udp.request.message-length", String(message.size()));

  if (serviceId == serviceIds::_reserved_event) {
    if (message.empty() || message[0] == 0) {
      state.eventPeer.port = 0;
      state.debugCallback("udp.request.set-event-peer", "clear");
    } else {
      state.eventPeer = peer;

      state.debugCallback("udp.request.set-event-peer", "set");
      state.debugCallback("udp.request.set-event-peer.ip", peer.ip.toString());
      state.debugCallback("udp.request.set-event-peer.port", String(peer.port));
    }

    return;
  }

  bool match = false;
  std::for_each(
    std::begin(state.services),
    std::end(state.services),
    [&](UDPService *service) {
      if (service->serviceId != serviceId) return;
      match = true;

      service->handler(
        &message,
        [this, peer, messageId, serviceId](std::vector<uint8_t> response) {
          std::vector<uint8_t> outgoing = {
            messageId
          };

          uint8_t *responseStart = response.data();
          uint8_t *responseEnd = responseStart + response.size();
          outgoing.insert(outgoing.end(), responseStart, responseEnd);

          state.debugCallback("udp.response.length", String(outgoing.size()));
          state.debugCallback("udp.response.remote-ip", peer.ip.toString());
          state.debugCallback("udp.response.remote-port", String(peer.port));
          state.debugCallback("udp.response.message-id", String(messageId, HEX));
          state.debugCallback("udp.response.service-id", String(serviceId, HEX));
          state.debugCallback("udp.response.message-length", String(response.size()));

          state.udp.writeTo(
            outgoing.data(),
            outgoing.size(),
            peer.ip,
            peer.port
          );
        }
      );
    }
  );

  if (!match) {
    state.debugCallback("event", "udp.request.no-matching-service");
  }
}

bool UDPMessaging::hasEventPeer() {
  return !!state.eventPeer.port;
}

void UDPMessaging::setDebug(LoggingCallback callback) {
  state.debugCallback = callback;
}
