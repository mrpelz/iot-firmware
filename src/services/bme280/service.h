#ifndef _BME280_SERVICE
#define _BME280_SERVICE

#include <Arduino.h>

#include "../../utils/udp-messaging.h"
#include "../service-ids.h"
#include "./handler.h"

UDPService bme280Service = {
  .serviceId = serviceIds::bme280,
  .handler = makeBme280Handler(),
};

#endif
