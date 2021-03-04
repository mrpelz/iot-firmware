#ifdef IOT_NODE_TSL2561

#include "./sensor.h"

namespace IotNode {

namespace Tsl2561 {
  void Sensor::begin(TwoWire *i2c) {
    _i2c_address = TSL2561_CHOSEN_ADDR;
    _i2c = i2c;
  }

  boolean Sensor::clearInterrupt(void) {
    // Set up command byte for interrupt clear
    _i2c->beginTransmission(_i2c_address);
    _i2c->write(TSL2561_CMD_CLEAR);
    _error = _i2c->endTransmission();
    if (_error == 0)
      return(true);

    return(false);
  }

  boolean Sensor::readByte(unsigned char address, unsigned char &value) {
    // Set up command byte for read
    _i2c->beginTransmission(_i2c_address);
    _i2c->write((address & 0x0F) | TSL2561_CMD);
    _error = _i2c->endTransmission();

    // Read requested byte
    if (_error == 0)
    {
      _i2c->requestFrom(_i2c_address,1);
      if (_i2c->available() == 1)
      {
        value = _i2c->read();
        return(true);
      }
    }
    return(false);
  }

  boolean Sensor::writeByte(unsigned char address, unsigned char value) {
    // Set up command byte for write
    _i2c->beginTransmission(_i2c_address);
    _i2c->write((address & 0x0F) | TSL2561_CMD);
    // Write byte
    _i2c->write(value);
    _error = _i2c->endTransmission();
    if (_error == 0)
      return(true);

    return(false);
  }

  boolean Sensor::readUInt(unsigned char address, unsigned int &value) {
    char high, low;
    
    // Set up command byte for read
    _i2c->beginTransmission(_i2c_address);
    _i2c->write((address & 0x0F) | TSL2561_CMD);
    _error = _i2c->endTransmission();

    // Read two bytes (low and high)
    if (_error == 0)
    {
      _i2c->requestFrom(_i2c_address,2);
      if (_i2c->available() == 2)
      {
        low = _i2c->read();
        high = _i2c->read();
        // Combine bytes into unsigned int
        value = word(high,low);
        return(true);
      }
    }	
    return(false);
  }
}

} // project namespace

#endif
