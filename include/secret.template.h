// LINK
// wifi:credentials
// #define IOT_NODE_WIFI_SSID "<SSID>"
// #define IOT_NODE_WIFI_PSK "<PSK>"

// wifi:advanced (AP-pinning)
// #define IOT_NODE_AP_XYZ

#ifdef IOT_NODE_AP_XYZ
  #define IOT_NODE_WIFI_BSSID { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
  #define IOT_NODE_WIFI_CHANNEL 0
#endif
