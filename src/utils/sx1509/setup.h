#pragma once

#ifdef IOT_NODE_SX1509

#include <Arduino.h>
#include <Wire.h>

#include <SparkFunSX1509.h>

#include "../i2c.h"
#include "../log.h"

#define SX1509_ADDRESS 0x3E

#define ROOM_SENSOR_INDICATOR_LED_R 5
#define ROOM_SENSOR_INDICATOR_LED_G 6
#define ROOM_SENSOR_INDICATOR_LED_B 4

#define ROOM_SENSOR_BUZZER 7

namespace IotNode::Utils::Sx1509
{
  extern bool working;

  extern SX1509 io;

  void setup();
}

#endif
