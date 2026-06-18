#ifdef IOT_NODE_TCP_BRIDGE

#include "./tcp-bridge.h"

namespace IotNode::Setup::TCPBridge
{
  Config configs[] = {
      {
          .serial = {
              .port = &Serial1,
              .onSerialInit = [](HardwareSerial *serial)
              { serial->begin(9600, SERIAL_8N1, 32, 33, false); }},
          .server = {
              .port = 1337,
          },
      },
      {
          .serial = {.port = &Serial2, .onSerialInit = [](HardwareSerial *serial)
                                       { serial->begin(9600, SERIAL_8N1, 5, 17, false); }},
          .server = {
              .port = 1338,
          },
      }};

  void onSerialData(Config *config)
  {
    size_t length = config->serial.port->available();
    if (!length)
    {
      yield();
      return;
    }

#ifdef IOT_NODE_LOGGING
    ::IotNode::Utils::Log::debug(fmt::format("serial port for tcp port {} received data with length {}", config->server.port, length));
#endif

    unsigned char buffer[length];
    length = config->serial.port->read(buffer, length);

    if (!config->server.clients.size())
    {
      yield();
      return;
    }

    xRingbufferSend(config->server.serialToClientsBuffer, buffer, length, pdMS_TO_TICKS(1000));

#ifdef IOT_NODE_LOGGING
    ::IotNode::Utils::Log::debug(fmt::format("data from serial port for tcp port {} was enqueued", config->server.port));
#endif

    yield();
  }

  void onClient(void *arg, AsyncClient *client)
  {
    auto config = reinterpret_cast<Config *>(arg);

    client->setNoDelay(true);
    client->setRxTimeout(0);

    auto clientKey = fmt::format("{}:{}", client->remoteIP().toString().c_str(), client->remotePort());

    config->server.clients.emplace(clientKey, client);

#ifdef IOT_NODE_LOGGING
    ::IotNode::Utils::Log::debug(fmt::format("tcp client {} connected\n{} tcp client{} connected", clientKey, config->server.clients.size(), config->server.clients.size() == 1 ? "" : "s"));
#endif

    client->onData([clientKey, config](void *arg, AsyncClient *client, void *buffer, size_t length)
                   {
#ifdef IOT_NODE_LOGGING
                       ::IotNode::Utils::Log::debug(fmt::format("tcp client {} received data with length {}", clientKey, length));
#endif
                       client->ackLater();
                       xRingbufferSend(config->serial.clientsToSerialBuffer, buffer, length, pdMS_TO_TICKS(1000));
                       client->ack(length);

#ifdef IOT_NODE_LOGGING
                       ::IotNode::Utils::Log::debug(fmt::format("data from tcp client {} was enqueued", clientKey));
#endif
                       yield(); });

    client->onError([clientKey](void *arg, AsyncClient *client, int8_t error)
                    {
#ifdef IOT_NODE_LOGGING
                      ::IotNode::Utils::Log::debug(fmt::format("tcp error \"{}\" \nfor client {}", client->errorToString(error), clientKey));
#endif
                    });
    client->onTimeout([clientKey](void *arg, AsyncClient *client, uint32_t time)
                      {
#ifdef IOT_NODE_LOGGING
                        ::IotNode::Utils::Log::debug(fmt::format("tcp ACK timeout for client {}", clientKey));
#endif
                      });
    client->onDisconnect([clientKey, config](void *arg, AsyncClient *client)
                         {
                           config->server.clients.erase(clientKey);
#ifdef IOT_NODE_LOGGING
                           ::IotNode::Utils::Log::debug(fmt::format("tcp client {} disconnected\n{} client{} connected", clientKey, config->server.clients.size(), config->server.clients.size() == 1 ? "" : "s"));
#endif
                         });

    yield();
  }

  void maintenanceTask(void *parameter)
  {
    auto config = reinterpret_cast<Config *>(parameter);

    config->serial.port->onReceive([&config]()
                                   { onSerialData(config); });

    config->server.instance->onClient(&onClient, config);

    for (;;)
    {
      while (!::IotNode::Utils::Link::link.isConnected())
      {
        vTaskDelay(pdMS_TO_TICKS(100));
      }

#ifdef IOT_NODE_LOGGING
      ::IotNode::Utils::Log::debug("link online");
#endif

      config->server.instance->begin();

#ifdef IOT_NODE_LOGGING
      ::IotNode::Utils::Log::debug(fmt::format("server listening on tcp port {}", config->server.port));
#endif

      while (::IotNode::Utils::Link::link.isConnected())
      {
        size_t lengthToClientsAvailable = 0;
        vRingbufferGetInfo(config->server.serialToClientsBuffer, NULL, NULL, NULL, NULL, &lengthToClientsAvailable);
        if (lengthToClientsAvailable)
        {
          for (const auto &[clientKey, client] : config->server.clients)
          {
            auto lengthToClientAvailable = client->canSend() ? client->space() : 0;

#ifdef IOT_NODE_LOGGING
            ::IotNode::Utils::Log::debug(fmt::format("tcp client {} can receive {} bytes", clientKey, lengthToClientAvailable));
#endif

            lengthToClientsAvailable = min(lengthToClientAvailable, lengthToClientsAvailable);
          }

#ifdef IOT_NODE_LOGGING
          ::IotNode::Utils::Log::debug(fmt::format("cumulatively, tcp clients on port {} can receive {} bytes each", config->server.port, lengthToClientsAvailable));
#endif
          if (lengthToClientsAvailable)
          {
            size_t lengthToClients;
            auto bufferToClients = xRingbufferReceiveUpTo(config->server.serialToClientsBuffer, &lengthToClients, pdMS_TO_TICKS(100), lengthToClientsAvailable);

            if (bufferToClients != NULL && lengthToClients)
            {
              for (const auto &[clientKey, client] : config->server.clients)
              {
                client->write((char *)bufferToClients, lengthToClients);
              }

              vRingbufferReturnItem(config->server.serialToClientsBuffer, bufferToClients);

#ifdef IOT_NODE_LOGGING
              ::IotNode::Utils::Log::debug(fmt::format("buffered data with length {} from serial port sent to tcp clients on port {}", lengthToClients, config->server.port));
#endif
            }
          }

          yield();
        }

        size_t lengthToSerialAvailable = 0;
        vRingbufferGetInfo(config->serial.clientsToSerialBuffer, NULL, NULL, NULL, NULL, &lengthToSerialAvailable);
        if (lengthToSerialAvailable)
        {
          size_t lengthToSerialAvailable_ = config->serial.port->availableForWrite();

#ifdef IOT_NODE_LOGGING
          ::IotNode::Utils::Log::debug(fmt::format("serial port for tcp port {} can receive {} bytes", config->server.port, lengthToSerialAvailable_));
#endif

          lengthToSerialAvailable = min(lengthToSerialAvailable_, lengthToSerialAvailable);

          if (lengthToSerialAvailable)
          {
            size_t lengthToSerial;
            auto bufferToSerial = xRingbufferReceiveUpTo(config->serial.clientsToSerialBuffer, &lengthToSerial, pdMS_TO_TICKS(100), lengthToSerialAvailable);

            if (bufferToSerial != NULL && lengthToSerial)
            {
              config->serial.port->write((unsigned char *)bufferToSerial, lengthToSerial);
              vRingbufferReturnItem(config->serial.clientsToSerialBuffer, bufferToSerial);

#ifdef IOT_NODE_LOGGING
              ::IotNode::Utils::Log::debug(fmt::format("buffered data with length {} from tcp port {} sent to serial port", lengthToSerial, config->server.port));
#endif
            }
          }
        }

        yield();
      }

#ifdef IOT_NODE_LOGGING
      ::IotNode::Utils::Log::debug("link offline");
#endif

      config->server.instance->end();

      vTaskDelay(pdMS_TO_TICKS(100));
    }
  }

  void setup()
  {
    vTaskDelay(pdMS_TO_TICKS(100));

    for (auto &config : configs)
    {
      config.serial.port->setRxBufferSize(SERIAL_RX_BUFFER_SIZE);
      config.serial.port->setTxBufferSize(SERIAL_TX_BUFFER_SIZE);
      config.serial.port->setRxTimeout(1);
      config.serial.port->setRxFIFOFull(1);
      config.serial.onSerialInit(config.serial.port);
      config.serial.clientsToSerialBuffer = xRingbufferCreate(CLIENTS_TO_SERIAL_BUFFER_SIZE, RINGBUF_TYPE_BYTEBUF);

      config.server.instance = new AsyncServer(config.server.port);
      config.server.serialToClientsBuffer = xRingbufferCreate(SERIAL_TO_CLIENTS_BUFFER_SIZE, RINGBUF_TYPE_BYTEBUF);

      xTaskCreatePinnedToCore(
          maintenanceTask,
          "tcp_bridge_maintenance",
          FREERTOS_STACK_SIZE,
          &config,
          tskIDLE_PRIORITY,
          NULL,
          CONFIG_ARDUINO_RUNNING_CORE);
    }
  }
}

#endif
