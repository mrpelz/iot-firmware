#pragma once

#ifdef IOT_NODE_TCP_BRIDGE

#include <Arduino.h>
#include <map>
#include <freertos/ringbuf.h>

#include <pre.h>
#include <post.h>

#include <WiFi.h>
#include <AsyncTCP.h>
#include <FmtLib.h>

#ifdef IOT_NODE_LOGGING
#include "../../utils/log.h"
#endif

#include "../../utils/link/setup.h"

#define SERIAL_RX_BUFFER_SIZE 2500
#define SERIAL_TX_BUFFER_SIZE 2500
#define SERIAL_TO_CLIENTS_BUFFER_SIZE 25000
#define CLIENTS_TO_SERIAL_BUFFER_SIZE 25000

namespace IotNode::Setup::TCPBridge
{
  typedef ::std::map<::std::string, AsyncClient *> Clients;

  struct SerialState
  {
    HardwareSerial *port;
    ::std::function<void(HardwareSerial *serial)> onSerialInit;
    RingbufHandle_t clientsToSerialBuffer;
  };

  struct ServerState
  {
    int port;
    AsyncServer *instance;
    Clients clients;
    RingbufHandle_t serialToClientsBuffer;
  };

  struct Config
  {
    SerialState serial;
    ServerState server;
  };

  void onSerialData(Config *config);

  void onClient(void *arg, AsyncClient *instance);

  void maintenanceTask(void *parameter);

  void setup();
}

#endif
