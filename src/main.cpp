#include "./main.h"

void setup() {
  IotNode::Utils::Keepalive::setup();

  #ifdef IOT_NODE_LOGGING
    IotNode::Utils::Log::setup();
  #endif

  #ifdef IOT_NODE_ESP8266
    delay(IOT_NODE_LOG_DELAY);
  #endif
  #ifdef IOT_NODE_ESP32
    vTaskDelay(IOT_NODE_LOG_DELAY / portTICK_PERIOD_MS);
  #endif

  #ifdef IOT_NODE_INDICATORS
    IotNode::Utils::Indicator::setup();
    IotNode::Utils::Indicator::indicator0.setOn(true);
  #endif

  #ifdef IOT_NODE_LED
    IotNode::Utils::Led::setup();
  #endif

  #ifdef IOT_NODE_RELAIS
    IotNode::Utils::Relais::setup();
  #endif

  #ifdef IOT_NODE_BUTTONS
    IotNode::Utils::Button::setup();
  #endif

  IotNode::Utils::UDP::setup();
  IotNode::Utils::Link::setup();

  #ifdef IOT_NODE_ESP32
    IotNode::Services::Async::setup();
  #endif

  IotNode::Services::Hello::setup();
  IotNode::Services::Keepalive::setup();
  IotNode::Services::SystemInfo::setup();

  #ifdef IOT_NODE_INDICATORS
    IotNode::Services::Indicator::setup();
  #endif

  #ifdef IOT_NODE_LED
    IotNode::Services::Led::setup();
  #endif

  #ifdef IOT_NODE_RELAIS
    IotNode::Services::Relais::setup();
  #endif

  #ifdef IOT_NODE_BUTTONS
    IotNode::Events::Button::setup();
  #endif

  #ifdef IOT_NODE_MOTION_SENSOR
    IotNode::Events::MotionSensor::setup();
  #endif

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

  IotNode::Utils::OTA::setup();

  #ifdef IOT_NODE_INDICATORS
    if (IotNode::Utils::Link::link.isConnected()) return;

    IotNode::Utils::Indicator::indicator0.blink();
  #endif
}

void loop() {
  #ifdef IOT_NODE_ESP8266
    delay(IOT_NODE_MUTLITASKING_DELAY);

    #ifdef IOT_NODE_INDICATORS
      IotNode::Utils::Indicator::update();
    #endif

    #ifdef IOT_NODE_LED
      IotNode::Utils::Led::update();
    #endif

    #ifdef IOT_NODE_BUTTONS
      IotNode::Utils::Button::update();
    #endif

    #ifdef IOT_NODE_MOTION_SENSOR
      IotNode::Events::MotionSensor::update();
    #endif

    IotNode::Utils::Keepalive::update();
    IotNode::Utils::Link::update();
    IotNode::Utils::OTA::update();
  #endif
  #ifdef IOT_NODE_ESP32
    vTaskDelay(IOT_NODE_MUTLITASKING_DELAY / portTICK_PERIOD_MS);
  #endif
}
