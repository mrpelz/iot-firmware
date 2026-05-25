#pragma once

#ifdef IOT_NODE_E_PAPER

#include <Arduino.h>

#include <pre.h>
#include <post.h>
#include <functional>

#include "epd_driver.h"
#include "esp_websocket_client.h"
#include <touch.h>

#include "../../utils/log.h"
#include "../i2c.h"

#define E_PAPER_FRAMEBUFFER_SIZE EPD_WIDTH *EPD_HEIGHT / 2
#define E_PAPER_CLEANUP_EVERY_NTH_UPDATE 50
#define E_PAPER_TOUCH_INTERRUPT_PIN 13

#define WS_URI_ORIGIN "10.97.0.192:1337"
// #define WS_URI_URL "https\%3A\%2F\%2Fiot.i.wurstsalat.cloud\%2F\%23dao\%3D0\%26swo\%3D0\%26olo\%3D1\%26pao\%3D7"
#define WS_URI_URL "https\%3A\%2F\%2Fnews.ycombinator.com"
#define WS_URI_COLORS 16
#define WS_URI_DEPTH 15
#define WS_URI_DPR 2
#define WS_URI_TIMEOUT 1000
#define WS_URI "ws://" WS_URI_ORIGIN "?url=" WS_URI_URL "&colors=" IOT_NODE_STR(WS_URI_COLORS) "&width=" IOT_NODE_STR(EPD_WIDTH) "&height=" IOT_NODE_STR(EPD_HEIGHT) "&depth=" IOT_NODE_STR(WS_URI_DEPTH) "&dpr=" IOT_NODE_STR(WS_URI_DPR) "&timeout=" IOT_NODE_STR(WS_URI_TIMEOUT)

#define WS_BUFFER_SIZE 5000
#define WS_HEADER_SIZE 16
#define WS_RX_BUFFER_SIZE E_PAPER_FRAMEBUFFER_SIZE + WS_HEADER_SIZE

namespace IotNode::Utils::EPaper
{
  void epdClear();

  bool touchSetup();

  bool epdSetup();

  void websocketDataHandler(esp_websocket_event_data_t *data);

  void websocketEventHandler(
      void *handler_args,
      esp_event_base_t base,
      long event_id,
      void *event_data);

  void websocketSend(unsigned long type);

  void websocketTouch();

  bool websocketStart();
}

#endif
