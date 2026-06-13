#include "./main.h"

void setup()
{
#ifdef IOT_NODE_ESP_NOW_NODE
  ::IotNode::Utils::EspNowNode::getWorkingMode();

  if (::IotNode::Utils::EspNowNode::workingMode == ::IotNode::Utils::EspNowNode::WORKING_MODE::SLEEP)
  {
#ifdef IOT_NODE_BUTTONS
    ::IotNode::Utils::Button::setup();
#endif

    ::IotNode::Utils::EspNowNode::setup();

#ifdef IOT_NODE_BUTTONS
    ::IotNode::Events::Button::setupEspNow();
#endif

#ifdef IOT_NODE_INPUT
    ::IotNode::Events::Input::setupEspNow();
#endif

#ifdef IOT_NODE_VCC
    ::IotNode::Events::VCC::setupEspNow();
#endif

    return;
  }
#endif

  ::IotNode::Utils::Keepalive::setup();

#ifdef IOT_NODE_LOGGING
  ::IotNode::Utils::Log::setup();
#endif

#ifdef IOT_NODE_ESP8266
  delay(IOT_NODE_LOG_DELAY);
#endif
#ifdef IOT_NODE_ESP32
  vTaskDelay(IOT_NODE_LOG_DELAY / portTICK_PERIOD_MS);
#endif

#ifdef IOT_NODE_I2C
  ::IotNode::Utils::I2C::setup();
#endif

#ifdef IOT_NODE_SX1509
  ::IotNode::Utils::Sx1509::setup();
#endif

#ifdef IOT_NODE_INDICATORS
  ::IotNode::Utils::Indicator::setup();
  ::IotNode::Utils::OutputNg::setup();
#if !defined(IOT_NODE_BOARD_WAVESHARE_ESP32_S3_ZERO) && !defined(IOT_NODE_BOARD_WAVESHARE_ESP32_S3_ETH)
  ::IotNode::Utils::Indicator::indicator0.setOn(true);
#endif
#endif

#ifdef IOT_NODE_LED
  ::IotNode::Utils::Led::setup();
#endif

#ifdef IOT_NODE_OUTPUT
  ::IotNode::Utils::Output::setup();
#endif

#ifdef IOT_NODE_BUTTONS
  ::IotNode::Utils::Button::setup();
#endif

  ::IotNode::Utils::UDP::setup();
  ::IotNode::Utils::Link::setup();

#ifdef IOT_NODE_ASYNC
  ::IotNode::Services::Async::setup();
#endif

  ::IotNode::Services::Hello::setup();
  ::IotNode::Services::Keepalive::setup();
  ::IotNode::Services::SystemInfo::setup();

#ifdef IOT_NODE_INDICATORS
  ::IotNode::Services::Indicator::setup();
#if defined(IOT_NODE_BOARD_WAVESHARE_ESP32_S3_ZERO) || defined(IOT_NODE_BOARD_WAVESHARE_ESP32_S3_ETH)
  ::IotNode::Services::OutputNg::setup();
#endif
#endif

#ifdef IOT_NODE_LED
  ::IotNode::Services::Led::setup();
#endif

#ifdef IOT_NODE_OUTPUT
  ::IotNode::Services::Output::setup();
#endif

#ifdef IOT_NODE_BUTTONS
  ::IotNode::Events::Button::setup();
#endif

#ifdef IOT_NODE_ESP_NOW_GW
  ::IotNode::Events::EspNowGw::setup();
#endif

#ifdef IOT_NODE_INPUT
  ::IotNode::Events::Input::setup();
#endif

#ifdef IOT_NODE_HMMD_MOTION
  ::IotNode::Events::HmmdMotion::setup();
#endif

#ifdef IOT_NODE_RF_433
  ::IotNode::Events::Rf433::setup();
#endif

#ifdef IOT_NODE_BME280
  ::IotNode::Services::Bme280::setup();
#endif

#ifdef IOT_NODE_CCS811
  ::IotNode::Services::Ccs811::setup();
#endif

#ifdef IOT_NODE_EQ3
  ::IotNode::Utils::EQ3::setup();
#endif

#ifdef IOT_NODE_MCP9808
  ::IotNode::Services::Mcp9808::setup();
#endif

#ifdef IOT_NODE_MHZ19
  ::IotNode::Services::Mhz19::setup();
#endif

#ifdef IOT_NODE_SDS011
  ::IotNode::Services::Sds011::setup();
#endif

#ifdef IOT_NODE_SGP30
  ::IotNode::Services::Sgp30::setup();
#endif

#ifdef IOT_NODE_TSL2561
  ::IotNode::Services::Tsl2561::setup();
#endif

#ifdef IOT_NODE_VEML6070
  ::IotNode::Services::Veml6070::setup();
#endif

#ifdef IOT_NODE_E_PAPER
  ::IotNode::Utils::EPaper::setup();
  ::IotNode::Services::EPaper::setup();
#endif

#ifndef IOT_NODE_NO_OTA
  ::IotNode::Utils::OTA::setup();
#endif

#ifdef IOT_NODE_I2C_SCAN
  ::IotNode::Utils::I2C::scan();
#endif

#ifdef IOT_NODE_INDICATORS
  if (::IotNode::Utils::Link::link.isConnected())
    return;

#if !defined(IOT_NODE_BOARD_WAVESHARE_ESP32_S3_ZERO) && !defined(IOT_NODE_BOARD_WAVESHARE_ESP32_S3_ETH)
  ::IotNode::Utils::Indicator::indicator0.blink();
#endif
#endif
}

void loop()
{
#ifdef IOT_NODE_ESP8266
  delay(IOT_NODE_MUTLITASKING_DELAY);

#ifdef IOT_NODE_INDICATORS
  ::IotNode::Utils::Indicator::update();
  ::IotNode::Utils::OutputNg::update();
#endif

#ifdef IOT_NODE_LED
  ::IotNode::Utils::Led::update();
#endif

#ifdef IOT_NODE_BUTTONS
  ::IotNode::Utils::Button::update();
#endif

#ifdef IOT_NODE_INPUT
  ::IotNode::Events::Input::update();
#endif

#ifdef IOT_NODE_VCC
  ::IotNode::Utils::VCC::update();
  ::IotNode::Events::VCC::update();
#endif

#ifdef IOT_NODE_ESP_NOW_NODE
  if (::IotNode::Utils::EspNowNode::workingMode == ::IotNode::Utils::EspNowNode::WORKING_MODE::SLEEP)
  {
    ::IotNode::Utils::EspNowNode::update();
  }

  if (::IotNode::Utils::EspNowNode::workingMode == ::IotNode::Utils::EspNowNode::WORKING_MODE::WAKE)
  {
    ::IotNode::Utils::Keepalive::update();
    ::IotNode::Utils::Link::update();
    ::IotNode::Utils::OTA::update();
  }
#else
  ::IotNode::Utils::Keepalive::update();
  ::IotNode::Utils::Link::update();
#endif
#endif

#ifdef IOT_NODE_HMMD_MOTION
  ::IotNode::Events::HmmdMotion::update();
#endif

#ifndef IOT_NODE_NO_OTA
  ::IotNode::Utils::OTA::update();
#endif

#ifdef IOT_NODE_ESP32
  vTaskDelay(IOT_NODE_MUTLITASKING_DELAY / portTICK_PERIOD_MS);
#endif
}
