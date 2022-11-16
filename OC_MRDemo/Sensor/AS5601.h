/**
  ******************************************************************************
  * @file           : AS5601.h
  * @brief          : encoder
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
#ifndef AS5601_h
#define AS5601_h

#include "mbed.h"
#include "ISensor.h"

#define AS5600_AS5601_DEV_ADDRESS      (0x36 << 1)
#define AS5600_AS5601_REG_RAW_ANGLE    0x0C
#define AS5600_AS5601_REG_ANGLE    0x0E


class AS5601 : public ISensor{
private:
    I2C *_i2c;
public:
    AS5601(I2C *i2c, char *name);
    
    virtual uint16_t get_raw();
    virtual float get_angle();
};

#endif/*AS5601_h*/
