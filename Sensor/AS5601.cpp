/**
  ******************************************************************************
  * @file           : AS5601.cpp
  * @brief          : encoder
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
#include "AS5601.h"

/**
  * @brief constructor
  * @param  i2c:i2c  name:this name
  * @retval none
  */
AS5601::AS5601(I2C *i2c, char *name) : ISensor(name){
    _i2c = i2c;
    _i2c->frequency(400000); //400K or 100K
}
/**
  * @brief get raw(AD)
  * @param  none
  * @retval raw
  */
uint16_t AS5601::get_raw(){
    int ret;
    char cmd[2];
    cmd[0] = AS5600_AS5601_REG_RAW_ANGLE;
    ret = _i2c->write(AS5600_AS5601_DEV_ADDRESS, cmd, 1, true);//no stop
    ret |= _i2c->read(AS5600_AS5601_DEV_ADDRESS, cmd, 2);
    if(ret == 0){
        uint16_t RawAngle = 0;
        RawAngle  = ((uint16_t)cmd[0] << 8) & 0x0F00;
        RawAngle |= (uint16_t)cmd[1];
        // Raw angle value (0x0000~0x0FFF) is stored in RawAngle
        return RawAngle;//0-4095
    }
    return 0xffff;//i2c error
}
/**
  * @brief get angle
  * @param  none
  * @retval angle
  */
float AS5601::get_angle(){
    uint16_t raw = get_raw(); //0-4095
    float angle = raw * 360 / 4096;
    printf("[I2C] raw=%d, angle=%f\n", raw, angle);
    return angle;
}
