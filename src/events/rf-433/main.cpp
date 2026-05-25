#include "./main.h"

#ifdef IOT_NODE_RF_433

namespace IotNode::Events::Rf433
{
  void event(::std::vector<unsigned char> payload)
  {
    Utils::UDP::instance.event(ids::rf433, 0, payload);
  }

  void onDataReceived(unsigned int *protocol, unsigned int *bitLength, unsigned long *value)
  {
    ::std::vector<unsigned char> payload = {};

    auto _protocol = reinterpret_cast<unsigned char *>(protocol);
    auto _value = reinterpret_cast<unsigned char *>(value);

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
