#ifndef _BME280_SERVICE
#define _BME280_SERVICE

#include <Arduino.h>

#include "../../utils/udp.h"
#include "../service-ids.h"
#include "./handler.h"

namespace IotNode {

UDPService bme280Service = {
  .serviceId = serviceIds::bme280,
  .handler = makeBme280Handler(),
};

} // project namespace

#endif
