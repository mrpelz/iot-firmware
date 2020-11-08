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
  state.debugCallback("event", "udp.listening");
  state.debugCallback("udp.listening.port", String(state.port));

  state.udp.listen(state.port);
}

void UDPMessaging::close() {
  state.debugCallback("event", "udp.listening");
  state.debugCallback("udp.listening", "close");

  state.udp.close();
}

void UDPMessaging::event(uint8_t eventId, std::vector<uint8_t> event) {
  if (!state.eventPeer.remoteAddress.isSet() || !state.eventPeer.remotePort) return;

  std::vector<uint8_t> outgoing = {
    serviceIds::_reserved_event,
    eventId
  };

  uint8_t *eventStart = event.data();
  uint8_t *eventEnd = eventStart + event.size();
  outgoing.insert(outgoing.end(), eventStart, eventEnd);

  state.debugCallback("udp.event.length", String(outgoing.size()));
  state.debugCallback("udp.event.remote-ip", state.eventPeer.remoteAddress.toString());
  state.debugCallback("udp.event.remote-port", String(state.eventPeer.remotePort));
  state.debugCallback("udp.event.event-id", String(eventId, HEX));
  state.debugCallback("udp.event.message-length", String(event.size()));

  state.udp.writeTo(
    outgoing.data(),
    outgoing.size(),
    state.eventPeer.remoteAddress,
    state.eventPeer.remotePort
  );
}

void UDPMessaging::handleRequest(AsyncUDPPacket *packet) {
  state.debugCallback("event", "udp.request");

  size_t length = packet->length();

  if (length < REQUEST_MIN_LENGTH) {
    packet->flush();
    return;
  }

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

  if (serviceId == serviceIds::_reserved_event) {
    state.eventPeer = {
      packet->remoteIP(),
      packet->remotePort()
    };

    state.debugCallback("udp.request.set-event-peer.ip", packet->remoteIP().toString());
    state.debugCallback("udp.request.set-event-peer.port", String(packet->remotePort()));

    packet->flush();
    return;
  }

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

void UDPMessaging::setDebug(std::function<void (const String &, const String &)> callback) {
  state.debugCallback = callback;
}
