#pragma once

#ifdef IOT_NODE_EQ3

#include <Arduino.h>
#include <BLEClient.h>
#include <BLEDevice.h>
#include <BLEScan.h>
#include <ctime>
#include <functional>
#include <queue>
#include <sstream>
#include <string>

#include "eQ3_constants.h"
#include "eQ3_message.h"
#include "eQ3_util.h"

#define BLE_UUID_SERVICE "58e06900-15d8-11e6-b737-0002a5d5c51b"
#define BLE_UUID_WRITE "3141dd40-15db-11e6-a24b-0002a5d5c51b"
#define BLE_UUID_READ "359d4820-15db-11e6-82bd-0002a5d5c51b"
#define LOCK_TIMEOUT 35

namespace IotNode::Lib::EQ3
{
  class Class : public BLEAdvertisedDeviceCallbacks, public BLEClientCallbacks /*, public BLERemoteCharacteristicCallbacks*/
  {
  private:
    ::std::string _address;
    BLEScan *_bleScan;
    BLEAdvertisedDevice *_device;
    time_t _lastActivity = 0;
    SemaphoreHandle_t _mutex;
    ::std::function<void(LockStatus)> _onStatusChange;
    ::std::map<ConnectionState, ::std::function<void(void)>> _queue;
    ::std::vector<eQ3Message::MessageFragment> _recvFragments;
    BLERemoteCharacteristic *_recvChar;
    ::std::queue<eQ3Message::MessageFragment> _sendQueue;
    BLERemoteCharacteristic *_sendChar;

    friend BLEClient;
    friend BLEScan;

    void _connectHandler();
    void _exchangeNonces();
    void _onConnect(BLEClient *pClient);
    void _onDisconnect(BLEClient *pClient);
    void _onNotify(BLERemoteCharacteristic *pBLERemoteCharacteristic, uint8_t *pData, size_t length, bool isNotify);
    void _onResult(BLEAdvertisedDevice advertisedDevice);
    void _sendCommand(CommandType command);
    bool _sendMessage(eQ3Message::Message *msg);
    void _sendNextFragment();

  public:
    BLEClient *bleClient;
    int lockStatus;
    int rssi;
    ClientState state;
    ::std::string raw_data;
    Class(std::string ble_address, ::std::string user_key, unsigned char user_id = 0xFF);
    void connect();
    void lock();
    bool onTick();
    void open();
    void pairingRequest(std::string cardkey);
    void setOnStatusChange(std::function<void(LockStatus)> cb);
    void toggle();
    void unlock();
    void updateInfo();
  };
}

#endif
