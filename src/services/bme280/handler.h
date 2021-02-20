#ifndef _BME280_HANDLER
#define _BME280_HANDLER

#include <Arduino.h>
#include <Wire.h>

#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#include "../../utils/udp/main.h"
#include "../../utils/log.h"

namespace IotNode {

UDP::RequestHandler makeBme280Handler();

} // project namespace

#endif
