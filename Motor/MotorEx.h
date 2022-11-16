/**
  ******************************************************************************
  * @file           : MotorEx.h
  * @brief          : Motor Class (TEST)
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
#ifndef MOTOREX_h
#define MOTOREX_h

#include "mbed.h"
#include "Motor.h"

class MotorEx: public Motor{
private:
    int TIM_PERIOD;
    Ticker _tick;
    uint8_t _tim;
    uint8_t dir;
public:
    MotorEx(PwmOut *pwm, DigitalOut *dir, int period);
    virtual void pwmout(float pwm);
private:
    void pwm_sync();
};


#endif/*MOTOREX_h*/

