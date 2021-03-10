#ifdef IOT_NODE_CCS811

#include "./sensor.h"

namespace IotNode {
namespace Services {

namespace Ccs811 {
  bool Sensor::begin(TwoWire *i2c) {
    _i2c = i2c;

    return Adafruit_CCS811::begin();
  }

  void Sensor::_i2c_init() {}

  void Sensor::read(uint8_t reg, uint8_t *buf, uint8_t num) {
    uint8_t pos = 0;

    // on arduino we need to read in 32 byte chunks
    while (pos < num) {

      uint8_t read_now = min((uint8_t)32, (uint8_t)(num - pos));
      _i2c->beginTransmission((uint8_t)_i2caddr);
      _i2c->write((uint8_t)reg + pos);
      _i2c->endTransmission();
      _i2c->requestFrom((uint8_t)_i2caddr, read_now);

      for (int i = 0; i < read_now; i++) {
        buf[pos] = _i2c->read();
        pos++;
      }
    }
  }

  void Sensor::write(uint8_t reg, uint8_t *buf, uint8_t num) {
    _i2c->beginTransmission((uint8_t)_i2caddr);
    _i2c->write((uint8_t)reg);
    _i2c->write((uint8_t *)buf, num);
    _i2c->endTransmission();
  }
}

} // section namespace
} // project namespace

#endif
