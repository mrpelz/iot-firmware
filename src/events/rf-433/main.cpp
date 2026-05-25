#include "./main.h"

#ifdef IOT_NODE_RF_433

namespace IotNode::Events::Rf433
{
  void event(::std::vector<uint8_t> payload)
  {
    Utils::UDP::instance.event(ids::rf433, 0, payload);
  }

  void onDataReceived(unsigned int *protocol, unsigned int *bitLength, unsigned long *value)
  {
    ::std::vector<uint8_t> payload = {};

    auto _protocol = reinterpret_cast<uint8_t *>(protocol);
    auto _value = reinterpret_cast<uint8_t *>(value);

    payload.insert(
        payload.end(),
        _protocol,
        _protocol + 1 // discard rest, numbers are going to be small
    );

    payload.insert(
        payload.end(),
        _value,
        _value + sizeof(*value));

    event(payload);
  }
}

#endif
