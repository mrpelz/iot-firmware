#include "./main.h"

void setup() {
  IotNode::Utils::Log::setup();

  IotNode::Utils::Indicator::setup();
  IotNode::Utils::Indicator::rxdLed.setOn(true);

  IotNode::Utils::Relais::setup();

  IotNode::Utils::Button::setup();

  IotNode::Utils::UDP::setup();
  IotNode::Utils::Link::setup();

  #ifdef IOT_NODE_ESP32
    IotNode::Services::Async::setup();
  #endif

  IotNode::Services::Hello::setup();
  IotNode::Services::Keepalive::setup();
  IotNode::Services::SystemInfo::setup();

  IotNode::Services::Indicator::setup();
  IotNode::Services::Relais::setup();

  IotNode::Events::Button::setup();
  IotNode::Events::MotionSensor::setup();

  #ifdef IOT_NODE_I2C
    IotNode::Utils::I2C::setup();
  #endif

  #ifdef IOT_NODE_I2C_SCAN
    IotNode::Utils::I2C::scan();
  #endif

  #ifdef IOT_NODE_BME280
    IotNode::Services::Bme280::setup();
  #endif

  #ifdef IOT_NODE_CCS811
    IotNode::Services::Ccs811::setup();
  #endif

  #ifdef IOT_NODE_MCP9808
    IotNode::Services::Mcp9808::setup();
  #endif

  #ifdef IOT_NODE_MHZ19
    IotNode::Services::Mhz19::setup();
  #endif

  #ifdef IOT_NODE_SDS011
    IotNode::Services::Sds011::setup();
  #endif

  #ifdef IOT_NODE_SGP30
    IotNode::Services::Sgp30::setup();
  #endif

  #ifdef IOT_NODE_TSL2561
    IotNode::Services::Tsl2561::setup();
  #endif

  #ifdef IOT_NODE_VEML6070
    IotNode::Services::Veml6070::setup();
  #endif

  if (IotNode::Utils::Link::link.isConnected()) return;
  IotNode::Utils::Indicator::rxdLed.blink();
}

void loop() {
  #ifdef IOT_NODE_ESP8266
    delay(IOT_NODE_MUTLITASKING_DELAY);
    IotNode::Utils::Indicator::update();
    IotNode::Utils::Button::update();
    IotNode::Services::Keepalive::update();
    IotNode::Utils::Link::update();
  #endif
  #ifdef IOT_NODE_ESP32
    vTaskDelay(IOT_NODE_MUTLITASKING_DELAY / portTICK_PERIOD_MS);
  #endif
}
