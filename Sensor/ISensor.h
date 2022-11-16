/**
  ******************************************************************************
  * @file           : ISensor.h
  * @brief          : Sensor Interface
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
#ifndef ISENSOR_h
#define ISENSOR_h

#include "mbed.h"

class ISensor{
private:
    char _name[32];
public:
    virtual uint16_t get_raw()=0;
    virtual float get_angle()=0;
    char *name(){ return _name; }
    ISensor(char *name){
        memcpy(_name, name, sizeof(_name));
    }
};

#endif/*ISENSOR_h*/
