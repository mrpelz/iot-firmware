#include "./main.h"

#ifdef IOT_NODE_E_PAPER

namespace IotNode {
namespace Utils {

namespace EPaper {
  auto touch = TouchClass();
  uint8_t *framebuffer = NULL;

  static esp_websocket_client_config_t websocketConfig;
  esp_websocket_client_handle_t websocketClient = NULL;

  uint8_t *rxBuffer = NULL;
  Rect_t updateArea = {
    .x = 0,
    .y = 0,
    .width = 0,
    .height =  0
  };

  auto isConnected = false;
  uint8_t update = 0;

  struct TouchState {
    bool down = false;
    bool moved = false;
    uint16_t x = 0;
    uint16_t y = 0;
  } touchState;

  void epdClear() {
    epd_poweron();

    epd_clear_area_cycles(epd_full_screen(), 50, 25);

    epd_poweroff();
  }

  void websocketReset() {
    esp_websocket_client_stop(websocketClient);
    epdClear();
    esp_websocket_client_start(websocketClient);
  }

  bool touchSetup() {
    I2C::claim();
    bool touchInit = touch.begin(I2C::bus);
    I2C::unclaim();

    if (!touchInit) {
      #ifdef IOT_NODE_LOGGING
        Utils::Log::debug("error", "touch init failed");
      #endif
    }

    return touchInit;
  }

  bool epdSetup() {
    epd_init();

    framebuffer = (uint8_t *)ps_calloc(sizeof(uint8_t), E_PAPER_FRAMEBUFFER_SIZE);
    if (!framebuffer) {
      #ifdef IOT_NODE_LOGGING
        Utils::Log::debug("error", "epd framebuffer allocation failed");
      #endif

      return false;
    }

    memset(framebuffer, 0xff, E_PAPER_FRAMEBUFFER_SIZE);

    return true;
  }

  void websocketDataHandler(esp_websocket_event_data_t *data) {
    auto isFirstPacket = !data->payload_offset;
    auto isLastPacket = (data->payload_offset + data->data_len) == data->payload_len;

    auto destinationPointer = isFirstPacket
      ? rxBuffer + data->payload_offset
      : rxBuffer + data->payload_offset - WS_HEADER_SIZE;
    auto chunkPointer = isFirstPacket ? data->data_ptr + WS_HEADER_SIZE : data->data_ptr;
    auto chunkLength = isFirstPacket ? data->data_len - WS_HEADER_SIZE : data->data_len;

    #ifdef IOT_NODE_LOGGING
      Utils::Log::debug("websocket-event.isFirstPacket", String(isFirstPacket));
      Utils::Log::debug("websocket-event.isLastPacket", String(isLastPacket));
    #endif

    if (isFirstPacket) {
      auto x = ((uint32_t *)data->data_ptr)[0];
      auto y = ((uint32_t *)data->data_ptr)[1];
      auto width = ((uint32_t *)data->data_ptr)[2];
      auto height = ((uint32_t *)data->data_ptr)[3];

      #ifdef IOT_NODE_LOGGING
        Utils::Log::debug("websocket-event.x", String(x));
        Utils::Log::debug("websocket-event.y", String(y));
        Utils::Log::debug("websocket-event.width", String(width));
        Utils::Log::debug("websocket-event.height", String(height));
      #endif

      updateArea.x = x;
      updateArea.y = y;
      updateArea.width = width;
      updateArea.height = height;
    }

    memcpy(destinationPointer, chunkPointer, chunkLength);

    if (isLastPacket) {
      auto isSignificantUpdate = (
        updateArea.width > (EPD_WIDTH / 2)
      ) || (
        updateArea.height > (EPD_HEIGHT / 2)
      );
      auto cleanup = isSignificantUpdate || !((update + 1) % E_PAPER_CLEANUP_EVERY_NTH_UPDATE);
      update = isSignificantUpdate ? update : update + 1;

      epd_poweron();

      epd_copy_to_framebuffer(updateArea, rxBuffer, framebuffer);

      epd_clear_area_cycles(cleanup ? epd_full_screen() : updateArea, 10, 10);
      epd_draw_grayscale_image(epd_full_screen(), framebuffer);

      epd_poweroff();

      memset(rxBuffer, 0xff, WS_RX_BUFFER_SIZE);
    }
  }

  void websocketEventHandler(
    void *handler_args,
    esp_event_base_t base,
    int32_t event_id,
    void *event_data
  ) {
    esp_websocket_event_data_t *data = (esp_websocket_event_data_t *)event_data;

    switch (event_id) {
      case WEBSOCKET_EVENT_CONNECTED:
        #ifdef IOT_NODE_LOGGING
          Utils::Log::debug("websocket-event", "connected");
        #endif

        isConnected = true;

        return;
      case WEBSOCKET_EVENT_DISCONNECTED:
        #ifdef IOT_NODE_LOGGING
          Utils::Log::debug("websocket-event", "disconnected");
        #endif

        if (isConnected) {
          websocketReset();

        }

        isConnected = false;

        return;
      case WEBSOCKET_EVENT_DATA:
        if (!data->payload_len || !data->data_len) return;

        #ifdef IOT_NODE_LOGGING
          Utils::Log::debug("websocket-event", "data");
          Utils::Log::debug("websocket-event.payload.length", String(data->payload_len));
          Utils::Log::debug("websocket-event.payload.offset", String(data->payload_offset));
          Utils::Log::debug("websocket-event.data.length", String(data->data_len));
        #endif

        websocketDataHandler(data);
        
        return;
      case WEBSOCKET_EVENT_ERROR:
        #ifdef IOT_NODE_LOGGING
          Utils::Log::debug("websocket-event", "error");
        #endif

        if (isConnected) {
          websocketReset();
        }

        isConnected = false;
    }
  }

  void websocketTouch() {
    auto previouslyDown = touchState.down;

    I2C::claim();

    if (!touch.scanPoint()) {
      touchState.down = false;
      touchState.moved = false;
    } else {
      uint16_t x;
      uint16_t y;

      touch.getPoint(x, y, 0);
      y = EPD_HEIGHT - y;

      touchState.moved = touchState.down && touchState.x != x && touchState.y != y;
      touchState.down = true;
      touchState.x = x;
      touchState.y = y;
    }

    I2C::unclaim();

    if (previouslyDown == touchState.down && !touchState.moved) return;

    #ifdef IOT_NODE_LOGGING
      Utils::Log::debug("touch.down", String(touchState.down));
      Utils::Log::debug("touch.moved", String(touchState.moved));
      Utils::Log::debug("touch.x", String(touchState.x));
      Utils::Log::debug("touch.y", String(touchState.y));
    #endif

    if (!isConnected) return;

    uint32_t _type;
    if (!touchState.down) _type = 0;
    else if (touchState.moved) _type = 2;
    else _type = 1;

    uint32_t _x = touchState.x;
    uint32_t _y = touchState.y;

    auto __type = reinterpret_cast<uint8_t*>(&_type);
    auto __x = reinterpret_cast<uint8_t*>(&_x);
    auto __y = reinterpret_cast<uint8_t*>(&_y);

    std::vector<char> payload;

    payload.insert(
      payload.end(),
      __type,
      __type + sizeof(_type)
    );
    payload.insert(
      payload.end(),
      __x,
      __x + sizeof(_x)
    );
    payload.insert(
      payload.end(),
      __y,
      __y + sizeof(_y)
    );

    esp_websocket_client_send_bin(
      websocketClient,
      payload.data(),
      payload.size(),
      IOT_NODE_MUTLITASKING_DELAY / portTICK_PERIOD_MS
    );
  }

  bool websocketStart() {
    epdClear();

    rxBuffer = (uint8_t *)ps_calloc(sizeof(uint8_t), WS_RX_BUFFER_SIZE);
    if (!rxBuffer) {
      #ifdef IOT_NODE_LOGGING
        Utils::Log::debug("error", "rxBuffer allocation failed");
      #endif

      return false;
    }

    memset(rxBuffer, 0xff, WS_RX_BUFFER_SIZE);

    websocketConfig.uri = WS_URI;
    websocketConfig.buffer_size = WS_BUFFER_SIZE;

    websocketClient = esp_websocket_client_init(&websocketConfig);

    esp_websocket_register_events(websocketClient, WEBSOCKET_EVENT_ANY, websocketEventHandler, (void *)websocketClient);
    esp_websocket_client_start(websocketClient);

    return true;
  }
}

} // section namespace
} // project namespace

#endif
