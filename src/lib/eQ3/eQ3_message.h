#pragma once

#ifdef IOT_NODE_EQ3

#include <math.h>
#include <sstream>
#include <string>
#include <time.h>

#include "eQ3_constants.h"

namespace IotNode::Lib::EQ3
{
  class eQ3Message
  {
  public:
    class MessageFragment
    {
    public:
      ::std::string data;
      bool sent = false;
      time_t timeSent;
      ::std::string getData();
      char getStatusByte();
      char getType();
      int getRemainingFragmentCount();
      bool isComplete();
      bool isFirst();
      bool isLast();
    };

    class Message
    {
    public:
      char id;
      ::std::string data;
      Message(std::string data = "");
      virtual ::std::string encode(ClientState *state);
      bool isSecure();
      static bool isTypeSecure(char type);
      virtual void decode();
    };

    class Connection_Info_Message : public Message
    {
    public:
      Connection_Info_Message();
      char getUserId();
      ::std::string getRemoteSessionNonce();
      char getBootloaderVersion();
      char getAppVersion();
    };

    class Status_Changed_Message : public Message
    {
    public:
      Status_Changed_Message();
    };

    class Status_Info_Message : public Message
    {
    public:
      Status_Info_Message();
      int getLockStatus();
      int getUserRightType();
    };

    class StatusRequestMessage : public Message
    {
    public:
      StatusRequestMessage();
      ::std::string encode(ClientState *state) override;
    };

    class Connection_Close_Message : public Message
    {
    public:
      Connection_Close_Message();
    };

    class Connection_Request_Message : public Message
    {
    public:
      Connection_Request_Message();
      ::std::string encode(ClientState *state) override;
    };

    class CommandMessage : public Message
    {
    public:
      char command;
      CommandMessage(char command);
      ::std::string encode(ClientState *state) override;
    };

    class AnswerWithoutSecurityMessage : public Message
    {
    public:
      AnswerWithoutSecurityMessage();
    };

    class AnswerWithSecurityMessage : public Message
    {
    public:
      AnswerWithSecurityMessage();
      bool getA();
      bool getB();
    };

    class PairingRequestMessage : public Message
    {
    public:
      PairingRequestMessage();
      ::std::string encode(ClientState *state) override;
    };

    class FragmentAckMessage : public MessageFragment
    {
    public:
      char id;
      FragmentAckMessage(char fragment_id);
    };
  };
}

#endif
