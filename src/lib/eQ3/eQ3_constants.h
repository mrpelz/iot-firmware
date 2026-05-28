#pragma once

#ifdef IOT_NODE_EQ3

namespace IotNode::Lib::EQ3
{
  enum ConnectionState
  {
    DISCONNECTED = 0,
    SCANNING = 1,
    FOUND = 2,
    CONNECTING = 3,
    CONNECTED = 4,
    NONCES_EXCHANGED = 5
  };

  struct ClientState
  {
    char user_id;
    ::std::string user_key;
    ::std::string local_session_nonce;
    ::std::string remote_session_nonce;
    uint16_t local_security_counter = 1;
    uint16_t remote_security_counter = 0;
    ConnectionState connectionState = DISCONNECTED;
  };

  enum CommandType
  {
    LOCK = 0,
    UNLOCK = 1,
    OPEN = 2
  };

  enum LockStatus
  {
    UNKNOWN = 0,
    MOVING = 1,
    UNLOCKED = 2,
    LOCKED = 3,
    OPENED = 4
  };
}

#endif
