#ifdef IOT_NODE_EQ3

#include "eQ3.h"

namespace IotNode::Lib::EQ3
{
#define SEMAPHORE_WAIT_TIME (10000 / portTICK_PERIOD_MS)

  // Important: Initialize BLEDevice::init(""); yourself
  Class::Class(std::string ble_address, ::std::string user_key, unsigned char user_id)
  {
    state.user_key = hexstring_to_string(user_key);
    // Serial.println(state.user_key.length());
    state.user_id = user_id;

    _mutex = xSemaphoreCreateMutex();
    this->_address = ble_address;

    // init BLE scan
    _bleScan = BLEDevice::getScan();
    _bleScan->setAdvertisedDeviceCallbacks(this);
    _bleScan->setActiveScan(true);
    _bleScan->setInterval(160);
    _bleScan->setWindow(200);

    // TODO move this out to an extra init?
    bleClient = BLEDevice::createClient();
    bleClient->setClientCallbacks((BLEClientCallbacks *)this);
  }

  void Class::onConnect(BLEClient *pClient)
  {
    Serial.println("# Connecting...");
    state.connectionState = CONNECTING;
  }

  void Class::onDisconnect(BLEClient *pClient)
  {
    Serial.println("# Disconnected!");
    state.connectionState = DISCONNECTED;
    _recvFragments.clear();
    _sendQueue = ::std::queue<eQ3Message::MessageFragment>(); // clear queue
    _queue.clear();
    _sendChar = _recvChar = nullptr;
  }

  bool Class::onTick()
  {
    // end task when disconnected?
    // if (state.connectionState == DISCONNECTED)
    //    return false;
    if (xSemaphoreTake(_mutex, 0))
    {
      if (state.connectionState == FOUND)
      {
        Serial.println("# Connecting in tick");
        bleClient->connect(BLEAddress(_address.c_str()), BLE_ADDR_TYPE_PUBLIC);
      }
      else if (state.connectionState == CONNECTING)
      {
        Serial.println("# Connected in tick");
        BLERemoteService *comm;
        comm = bleClient->getService(BLEUUID(BLE_UUID_SERVICE));
        _sendChar = comm->getCharacteristic(BLEUUID(BLE_UUID_WRITE)); // write buffer characteristic
        _recvChar = comm->getCharacteristic(BLEUUID(BLE_UUID_READ));  // read buffer characteristic
        // _recvChar->setNotifyCallbacks((BLERemoteCharacteristicCallbacks*)this);
        _recvChar->registerForNotify([this](BLERemoteCharacteristic *pBLERemoteCharacteristic, uint8_t *pData, size_t length, bool isNotify)
                                     { _onNotify(pBLERemoteCharacteristic, pData, length, isNotify); });
        _lastActivity = time(NULL);
        state.connectionState = CONNECTED;
        auto queueFunc = _queue.find(CONNECTED);
        if (queueFunc != _queue.end())
        {
          _queue.erase(CONNECTED);
          // xSemaphoreGive(_mutex); // function will take the semaphore again
          queueFunc->second();
        }
      }
      else
      {
        _sendNextFragment();
        _lastActivity = time(NULL);
      }
      // TODO disconnect if no answer for long time?
      /* if (state.connectionState >= CONNECTED && difftime(_lastActivity, time(NULL)) > LOCK_TIMEOUT && _sendQueue.empty()) {
           Serial.println("# Lock timeout");
           bleClient->disconnect();
       }*/
      xSemaphoreGive(_mutex);
    }
    return true;
  }

  void Class::onResult(BLEAdvertisedDevice advertisedDevice)
  {
    if (advertisedDevice.getAddress().toString().c_str() == _address.c_str())
    { // TODO: Make name and address variable
      Serial.print("# Found _device: ");
      Serial.println(advertisedDevice.getAddress().toString().c_str());
      Serial.print("# RSSI: ");
      Serial.println(advertisedDevice.getRSSI());
      rssi = advertisedDevice.getRSSI();
      _bleScan->stop();
      state.connectionState = FOUND;
    }
  }

  void Class::setOnStatusChange(std::function<void(LockStatus)> cb)
  {
    xSemaphoreTake(_mutex, SEMAPHORE_WAIT_TIME);
    _onStatusChange = cb;
    xSemaphoreGive(_mutex);
  }

  void Class::_exchangeNonces()
  {
    state.local_session_nonce.clear();
    for (int i = 0; i < 8; i++)
      state.local_session_nonce.append(1, esp_random());
    auto *msg = new eQ3Message::Connection_Request_Message;
    Serial.println("# Nonce exchange");
    _sendMessage(msg);
  }

  void Class::connect()
  {
    state.connectionState = SCANNING;
    _bleScan->start(25, nullptr, false);
    Serial.println("# Searching ...");
    // state.connectionState = FOUND;
    // Serial.println("connecting directly...");
  }

  bool Class::_sendMessage(eQ3Message::Message *msg)
  {
    ::std::string data;
    if (msg->isSecure())
    {
      if (state.connectionState < NONCES_EXCHANGED)
      {
        // TODO check if slot for nonces_exchanged is already set?
        _queue.insert(::std::make_pair(NONCES_EXCHANGED, [this, msg]
                                       {
                  Serial.println("# sendMessage called again...");
                  _sendMessage(msg); }));
        _exchangeNonces();
        return true;
      }

      ::std::string padded_data;
      padded_data.append(msg->encode(&state));
      int pad_to = generic_ceil(padded_data.length(), 15, 8);
      if (pad_to > padded_data.length())
        padded_data.append(pad_to - padded_data.length(), 0);
      crypt_data(padded_data, msg->id, state.remote_session_nonce, state.local_security_counter, state.user_key);
      data.append(1, msg->id);
      data.append(crypt_data(padded_data, msg->id, state.remote_session_nonce, state.local_security_counter, state.user_key));
      data.append(1, (char)(state.local_security_counter >> 8));
      data.append(1, (char)state.local_security_counter);
      data.append(compute_auth_value(padded_data, msg->id, state.remote_session_nonce, state.local_security_counter, state.user_key));
      state.local_security_counter++;
    }
    else
    {
      if (state.connectionState < CONNECTED)
      {
        // TODO check if slot for connected is already set?
        _queue.insert(::std::make_pair(CONNECTED, [this, msg]
                                       { _sendMessage(msg); }));
        connect();
        return true;
      }
      data.append(1, msg->id);
      data.append(msg->encode(&state));
    }
    // fragment
    int chunks = data.length() / 15;
    if (data.length() % 15 > 0)
      chunks += 1;
    for (int i = 0; i < chunks; i++)
    {
      eQ3Message::MessageFragment frag;
      frag.data.append(1, (i ? 0 : 0x80) + (chunks - 1 - i)); // fragment status byte
      frag.data.append(data.substr(i * 15, 15));
      if (frag.data.length() < 16)
        frag.data.append(16 - (frag.data.length() % 16), 0); // padding
      _sendQueue.push(frag);
    }
    Serial.println("# sendMessage end.");
    ;
    free(msg);
    return true;
  }

  void Class::_onNotify(BLERemoteCharacteristic *pBLERemoteCharacteristic, uint8_t *pData, size_t length, bool isNotify)
  {
    xSemaphoreTake(_mutex, SEMAPHORE_WAIT_TIME);
    eQ3Message::MessageFragment frag;
    _lastActivity = time(NULL);
    frag.data = ::std::string((char *)pData, length);
    _recvFragments.push_back(frag);
    Serial.print("# Fragment Data: ");
    Serial.println(string_to_hex(frag.data).c_str());
    if (frag.isLast())
    {
      if (!_sendQueue.empty())
        _sendQueue.pop();
      // concat message
      ::std::stringstream ss;
      auto msgtype = _recvFragments.front().getType();
      for (auto &received_fragment : _recvFragments)
      {
        ss << received_fragment.getData();
      }
      ::std::string msgdata = ss.str();
      _recvFragments.clear();
      if (eQ3Message::Message::isTypeSecure(msgtype))
      {
        auto msg_security_counter = static_cast<uint16_t>(msgdata[msgdata.length() - 6]);
        msg_security_counter <<= 8;
        msg_security_counter += msgdata[msgdata.length() - 5];
        // Serial.println((int)msg_security_counter);
        if (msg_security_counter <= state.remote_security_counter)
        {
          Serial.print("# Msg. security counter: ");
          Serial.println(msg_security_counter);
          Serial.print("# Security counter: ");
          Serial.println(state.remote_security_counter);
          Serial.println("# Falscher remote counter");
          xSemaphoreGive(_mutex);
          return;
        }
        state.remote_security_counter = msg_security_counter;
        ::std::string msg_auth_value = msgdata.substr(msgdata.length() - 4, 4);
        Serial.print("# Auth value: ");
        Serial.println(string_to_hex(msg_auth_value).c_str());
        // ::std::string decrypted = crypt_data(msgdata.substr(0, msgdata.length() - 6), msgtype,
        ::std::string decrypted = crypt_data(msgdata.substr(1, msgdata.length() - 7), msgtype, state.local_session_nonce, state.remote_security_counter, state.user_key);
        Serial.print("# Crypted data: ");
        Serial.println(string_to_hex(msgdata.substr(1, msgdata.length() - 7)).c_str());
        ::std::string computed_auth_value = compute_auth_value(decrypted, msgtype, state.local_session_nonce, state.remote_security_counter, state.user_key);
        if (msg_auth_value != computed_auth_value)
        {
          Serial.println("# Auth value mismatch");
          xSemaphoreGive(_mutex);
          return;
        }
        msgdata = decrypted;
        Serial.print("# Decrypted: ");
        Serial.println(string_to_hex(msgdata).c_str());
      }

      switch (msgtype)
      {
      case 0:
      {
        // fragment ack, remove first
        if (!_sendQueue.empty())
          _sendQueue.pop();
        xSemaphoreGive(_mutex);
        return;
      }

      case 0x81: // answer with security
        // TODO call callback to user that pairing succeeded
        Serial.println("# Answer with security...");
        break;

      case 0x01: // answer without security
        // TODO report error
        Serial.println("# Answer without security...");
        break;

      case 0x05:
      {
        // status changed notification
        Serial.println("# Status changed notification");
        // TODO request status
        auto *message = new eQ3Message::StatusRequestMessage;
        _sendMessage(message);
        break;
      }

      case 0x03:
      {
        // connection info message
        eQ3Message::Connection_Info_Message message;
        message.data = msgdata;
        state.user_id = message.getUserId();
        state.remote_session_nonce = message.getRemoteSessionNonce();
        assert(state.remote_session_nonce.length() == 8);
        state.local_security_counter = 1;
        state.remote_security_counter = 0;
        state.connectionState = NONCES_EXCHANGED;

        Serial.println("# Nonce exchanged");
        auto queueFunc = _queue.find(NONCES_EXCHANGED);
        if (queueFunc != _queue.end())
        {
          _queue.erase(queueFunc);
          // xSemaphoreGive(_mutex); // function will take the semaphore again
          queueFunc->second();
        }
        xSemaphoreGive(_mutex);
        return;
      }

      case 0x83:
      {
        // status info
        eQ3Message::Status_Info_Message message;
        message.data = msgdata;
        Serial.print("# New state: ");
        Serial.println(message.getLockStatus());
        lockStatus = message.getLockStatus();
        raw_data = message.data;
        // _onStatusChange((LockStatus)message.getLockStatus()); // BUG: löst einen Reset aus!!
        break;
      }

      default:
        /*case 0x8f: */ { // user info
          Serial.println("# User info");
          xSemaphoreGive(_mutex);
          return;
        }
      }
    }
    else
    {
      // create ack
      Serial.println("# send Ack ");
      eQ3Message::FragmentAckMessage ack(frag.getStatusByte());
      _sendQueue.push(ack);
    }
    xSemaphoreGive(_mutex);
  }

  void Class::pairingRequest(std::string cardkey)
  {
    xSemaphoreTake(_mutex, SEMAPHORE_WAIT_TIME);
    if (state.connectionState < NONCES_EXCHANGED)
    {
      // TODO check if slot for nonces_exchanged is already set?

      // TODO callback when pairing finished, or make blocking?
      _queue.insert(::std::make_pair(NONCES_EXCHANGED, [this, cardkey]
                                     { pairingRequest(cardkey); }));
      Serial.println("# Pairing request");
      _exchangeNonces();
      xSemaphoreGive(_mutex);
      return;
    }
    auto *message = new eQ3Message::PairingRequestMessage();
    // return concatenated_array([data.user_id], padded_array(data.encrypted_pair_key, 22, 0), integer_to_byte_array(data.security_counter, 2), data.authentication_value);
    message->data.append(1, state.user_id);
    Serial.print("#Message id: ");
    Serial.println((int)message->id);

    // enc pair key
    assert(state.remote_session_nonce.length() == 8);
    assert(state.user_key.length() == 16);

    ::std::string card_key = hexstring_to_string(cardkey);

    ::std::string encrypted_pair_key = crypt_data(state.user_key, 0x04, state.remote_session_nonce, state.local_security_counter, card_key);
    if (encrypted_pair_key.length() < 22)
      encrypted_pair_key.append(22 - encrypted_pair_key.length(), 0);
    assert(encrypted_pair_key.length() == 22);
    message->data.append(encrypted_pair_key);

    // counter
    message->data.append(1, (char)(state.local_security_counter >> 8));
    message->data.append(1, (char)(state.local_security_counter));

    // auth value
    ::std::string extra;
    extra.append(1, state.user_id);
    extra.append(state.user_key);
    if (extra.length() < 23)
      extra.append(23 - extra.length(), 0);
    assert(extra.length() == 23);
    ::std::string auth_value = compute_auth_value(extra, 0x04, state.remote_session_nonce, state.local_security_counter, card_key);
    message->data.append(auth_value);
    assert(message->data.length() == 29);
    _sendMessage(message);
    xSemaphoreGive(_mutex);
  }

  void Class::_sendNextFragment()
  {
    if (_sendQueue.empty())
      return;
    if (_sendQueue.front().sent && ::std::difftime(_sendQueue.front().timeSent, ::std::time(NULL)) < 5)
      return;
    _sendQueue.front().sent = true;
    Serial.print("# Sending actual fragment: ");
    ::std::string data = _sendQueue.front().data;
    _sendQueue.front().timeSent = ::std::time(NULL);
    Serial.println(string_to_hex(data).c_str());
    assert(data.length() == 16);
    _sendChar->writeValue((uint8_t *)(data.c_str()), 16, true);
  }

  void Class::_sendCommand(CommandType command)
  {
    xSemaphoreTake(_mutex, SEMAPHORE_WAIT_TIME);
    Serial.println("# Getting Semaphore for sendcommand");
    auto msg = new eQ3Message::CommandMessage(command);
    _sendMessage(msg);
    Serial.println("# Semaphore handover");
    xSemaphoreGive(_mutex);
  }

  void Class::unlock()
  {
    _sendCommand(UNLOCK);
  }

  void Class::lock()
  {
    _sendCommand(LOCK);
  }

  void Class::open()
  {
    _sendCommand(OPEN);
  }

  void Class::updateInfo()
  {
    xSemaphoreTake(_mutex, SEMAPHORE_WAIT_TIME);
    auto *message = new eQ3Message::StatusRequestMessage;
    _sendMessage(message);
    xSemaphoreGive(_mutex);
  }
}

#endif
