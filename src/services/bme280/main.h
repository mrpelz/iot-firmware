#ifndef _BME280_MAIN
#define _BME280_MAIN

#ifdef IOT_NODE_BME280

#include <Arduino.h>
#include <Wire.h>

#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#include "../../utils/log.h"
#include "../../utils/udp/main.h"

namespace IotNode {

namespace Bme280 {
  void initializer(TwoWire *i2c);

  void responseTask(void *parameter);

  void handler(UDP::Payload *request, UDP::RespondCallback respond);
}

} // project namespace

#endif

#endif
