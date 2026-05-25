#pragma once

#ifdef IOT_NODE_ESP_NOW_NODE

#include <Arduino.h>
#include <vector>

#ifdef IOT_NODE_ESP8266
#include <ESP8266WiFi.h>
#include <espnow.h>
#endif
#ifdef IOT_NODE_ESP32
#include <esp_now.h>
#include <esp_wifi.h>
#include <WiFi.h>
#endif

#include "./button/setup.h"
#include "./keepalive/main.h"
#include "./log.h"

namespace IotNode::Utils::EspNowNode
{
  enum WORKING_MODE
  {
    SLEEP,
    WAKE
  };

  extern WORKING_MODE workingMode;

  void getWorkingMode();

  void sleep();

  void onDataSent(unsigned char *mac_addr, unsigned char status);

  void setup();

  void send(::std::vector<unsigned char> payload);

  void update();
}

#endif
