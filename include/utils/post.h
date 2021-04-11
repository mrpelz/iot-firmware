#ifndef IOT_NODE_NAME
  #define IOT_NODE_NAME IOT_NODE_HARDWARE_NAME
#endif

#if !defined(IOT_NODE_ESP32) && !defined(IOT_NODE_ESP8266)
  #error no valid platform specified
#endif

#if !defined(IOT_NODE_LINK_ETH) && !defined(IOT_NODE_LINK_WIFI)
  #error either eth or wifi must be specified
#endif

#if defined(IOT_NODE_LINK_ETH) && !defined(IOT_NODE_ESP32)
  #error cannot use ethernet without ESP32
#endif

#if defined(IOT_NODE_IP) && defined(IOT_NODE_GATEWAY) && defined(IOT_NODE_NETMASK)
  #define IOT_NODE_IP_STATIC
#else
  #define IOT_NODE_IP_DHCP
#endif

#if defined(IOT_NODE_LINK_ETH) && defined(IOT_NODE_LINK_WIFI)
  #error cannot specify both ethernet and wifi as link type
#endif

#ifndef IOT_NODE_LINK_ETH
  #undef IOT_NODE_WT32_ETH01
#endif

#if defined(IOT_NODE_LINK_WIFI) && (defined(IOT_NODE_AP_ALGORE) || defined(IOT_NODE_AP_ELZAR) || defined(IOT_NODE_AP_RICHARDNIXON) || defined(IOT_NODE_AP_SPIROAGNEW))
  #define IOT_NODE_ADVANCED_WIFI_CONFIG
#endif

#if defined(IOT_NODE_BME280) || defined(IOT_NODE_CCS811) || defined(IOT_NODE_MCP9808) || defined(IOT_NODE_SGP30) || defined(IOT_NODE_TSL2561) || defined(IOT_NODE_VEML6070)
  #define IOT_NODE_I2C_SENSOR
#endif

#if defined(IOT_NODE_MHZ19) || defined(IOT_NODE_SDS011)
  #define IOT_NODE_UART_SENSOR
#endif

#if !defined(IOT_NODE_ESP32) && (defined(IOT_NODE_I2C_SENSOR) || defined(IOT_NODE_UART_SENSOR))
  #error cannot use async sensors without ESP32
#endif

#if !defined(IOT_NODE_ESP32) && (defined(IOT_NODE_ESP_NOW_GW))
  #error ESP-NOW gateway only available on ESP32
#endif

#if !defined(IOT_NODE_LINK_ETH) && (defined(IOT_NODE_ESP_NOW_GW))
  #error ESP-NOW gateway only available when link is ethernet
#endif

#if defined(IOT_NODE_I2C_SENSOR) || defined(IOT_NODE_I2C_SCAN)
  #define IOT_NODE_I2C
#endif

#ifndef IOT_NODE_LOG_DELAY
  #define IOT_NODE_LOG_DELAY 250
#endif
