#ifndef _BME280_HANDLER
#define _BME280_HANDLER

#include <Arduino.h>
#include <Wire.h>

#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#include "../../utils/udp-messaging.h"

RequestHandler makeBme280Handler();

#endif
