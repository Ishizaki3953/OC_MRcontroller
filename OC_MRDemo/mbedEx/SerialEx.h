/**
  ******************************************************************************
  * @file           : SerialEx.h
  * @brief          : Serial Extension
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
#ifndef SERIALEX_h
#define SERIALEX_h

#include "mbed.h"
class SerialEx : public Serial{
private:
public:
    SerialEx(PinName tx, PinName rx, int baudrate) : Serial(tx, rx){
        baud(baudrate);
    }
};

#endif/*SERIALEX_h*/

