/**
  ******************************************************************************
  * @file           : Motor.h
  * @brief          : Motor Class
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
#ifndef MOTOR_h
#define MOTOR_h

#include "mbed.h"
#include "common.h"

#define PWM_PERIOD  (0.000025) //REVIEW 20us(50KHz)

class Motor{
protected:
    DigitalOut *_dir;
    PwmOut *_pwm;
public:
    int ZERO;
    Motor(PwmOut *pwm, DigitalOut *dir);
    virtual void pwmout(float pwm);
};



#endif/*MOTOR_h*/

