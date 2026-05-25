#include "./main.h"

namespace IotNode::Utils::UDP
{
  Class::Class(unsigned short port)
  {
    state.port = port;

    state.udp.onPacket([&](AsyncUDPPacket packet)
                       { handleRequest(&packet); });
  }

  void Class::addService(Service *service)
  {
    state.services.insert(::std::end(state.services), service);
  }

  void Class::begin()
  {
    if (state.isListening)
      return;

#ifdef IOT_NODE_LOGGING
    Log::debug("event: udp.listening");
    Log::debug(fmt::format("udp.listening.port: {}", state.port));
#endif

    state.isListening = true;
    state.udp.listen(state.port);
  }

  void Class::close()
  {
    if (!state.isListening)
      return;

#ifdef IOT_NODE_LOGGING
    Log::debug("event: udp.listening");
    Log::debug("udp.listening: close");
#endif

    removeEventPeer();

    state.isListening = false;
    state.udp.close();
  }

  void Class::event(unsigned char eventId, unsigned char eventIndex, Payload event)
  {
    if (!state.eventPeer.port)
      return;

    Payload outgoing;

    outgoing.insert(::std::end(outgoing), Services::ids::_reserved_event);
    outgoing.insert(::std::end(outgoing), eventId);
    outgoing.insert(::std::end(outgoing), eventIndex);
    outgoing.insert(::std::end(outgoing), ::std::begin(event), ::std::end(event));

#ifdef IOT_NODE_LOGGING
    Log::debug(fmt::format("udp.event.length: {}", outgoing.size()));
    Log::debug(fmt::format("udp.event.remote-ip: {}", state.eventPeer.ip.toString().c_str()));
    Log::debug(fmt::format("udp.event.remote-port: {}", state.eventPeer.port));
    Log::debug(fmt::format("udp.event.event-id: {:1X}", eventId));
    Log::debug(fmt::format("udp.event.event-index: {:1X}", eventIndex));
    Log::debug(fmt::format("udp.event.message-length: {}", event.size()));
#endif

    state.udp.writeTo(
        (unsigned char *)outgoing.data(),
        outgoing.size(),
        state.eventPeer.ip,
        state.eventPeer.port);
  }

  void Class::handleRequest(AsyncUDPPacket *packet)
  {
#ifdef IOT_NODE_LOGGING
    Log::debug("event: udp.request");
#endif

    size_t length = packet->length();

    if (length < REQUEST_MIN_LENGTH)
    {
      packet->flush();

      return;
    }

    Peer peer = {
        packet->remoteIP(),
        packet->remotePort()};

#ifdef IOT_NODE_LOGGING
    Log::debug(fmt::format("udp.request.incoming-ip: {}", peer.ip.toString().c_str()));
    Log::debug(fmt::format("udp.request.incoming-port: {}", peer.port));
#endif

    auto validPeer = peer.port && (peer.ip[0] + peer.ip[1] + peer.ip[2] + peer.ip[3]);
    if (validPeer)
    {
      state.fallbackPeer = peer;
    }
    else if (!state.fallbackPeer.port)
    {
#ifdef IOT_NODE_LOGGING
      Log::debug("event: udp.request.no-usable-peer");
#endif

      return;
    }
    else
    {
      peer = state.fallbackPeer;

#ifdef IOT_NODE_LOGGING
      Log::debug("event: udp.request.use-fallback-peer");
#endif
    }

    auto payload = packet->data();

    auto messageId = payload[0];
    auto messageVersion = payload[1];
    auto serviceId = payload[2];
    auto serviceIndex = payload[3];

    unsigned char *messageStart = payload + MESSAGE_ID_LENGTH + MESSAGE_VERSION_LENGTH + SERVICE_ID_LENGTH + SERVICE_INDEX_LENGTH;
    unsigned char *messageEnd = payload + length;

    Payload message(messageStart, messageEnd);

    packet->flush();

#ifdef IOT_NODE_LOGGING
    Log::debug(fmt::format("udp.request.length: {}", length));
    Log::debug(fmt::format("udp.request.remote-ip: {}", peer.ip.toString().c_str()));
    Log::debug(fmt::format("udp.request.remote-port: {}", peer.port));
    Log::debug(fmt::format("udp.request.message-id: {:1X}", messageId));
    Log::debug(fmt::format("udp.request.message-version: {:1X}", messageVersion));
    Log::debug(fmt::format("udp.request.service-id: {:1X}", serviceId));
    Log::debug(fmt::format("udp.request.service-index: {:1X}", serviceIndex));
    Log::debug(fmt::format("udp.request.message-length: {}", message.size()));
#endif

    if (messageVersion != MESSAGE_VERSION)
    {
#ifdef IOT_NODE_LOGGING
      Log::debug("event: udp.request.no-matching-version");
#endif

      return;
    }

    auto now = millis();
    auto timeSinceLastRequest = now - state.requestTimes.at(messageId);
    if (timeSinceLastRequest < REPEAT_WINDOW)
    {
#ifdef IOT_NODE_LOGGING
      Log::debug("event: udp.request.ignoring-repeated-message");
#endif

      return;
    }

    state.requestTimes[messageId] = now;

    bool match = false;
    ::std::for_each(
        ::std::begin(state.services),
        ::std::end(state.services),
        [&](Service *service)
        {
          if (match)
            return;
          if (service->serviceId != serviceId)
            return;
          if (service->serviceIndex != serviceIndex)
            return;

          match = true;

          service->handler(
              &message,
              [this, messageId, serviceId, peer](Payload response)
              {
                Payload outgoing;

                outgoing.insert(::std::end(outgoing), messageId);
                outgoing.insert(::std::end(outgoing), ::std::begin(response), ::std::end(response));

#ifdef IOT_NODE_LOGGING
                Log::debug(fmt::format("udp.response.length: {}", outgoing.size()));
                Log::debug(fmt::format("udp.response.remote-ip: {}", peer.ip.toString().c_str()));
                Log::debug(fmt::format("udp.response.remote-port: {}", peer.port));
                Log::debug(fmt::format("udp.response.message-id: {:1X}", messageId));
                Log::debug(fmt::format("udp.response.service-id: {:1X}", serviceId));
                Log::debug(fmt::format("udp.response.message-length: {}", response.size()));
#endif

                for (size_t i = 0; i < REPEAT_COUNT; i++)
                {
                  state.udp.writeTo(
                      (unsigned char *)outgoing.data(),
                      outgoing.size(),
                      peer.ip,
                      peer.port);
                }
              },
              peer);
        });

    if (!match)
    {
#ifdef IOT_NODE_LOGGING
      Log::debug("event: udp.request.no-matching-service");
#endif
    }
  }

  bool Class::hasEventPeer()
  {
    return !!state.eventPeer.port;
  }

  bool Class::isListening()
  {
    return !!state.isListening;
  }

  void Class::removeEventPeer()
  {
    state.eventPeer.port = 0;
  }

  void Class::setEventPeer(Peer peer)
  {
    state.eventPeer = peer;
  }
}
