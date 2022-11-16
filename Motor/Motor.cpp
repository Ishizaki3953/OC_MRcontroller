/**
  ******************************************************************************
  * @file           : Motor.cpp
  * @brief          : Motor Class
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
#include "Motor.h"
#include "common.h"

/**
  * @brief constructor
  * @param  pwm:pwm port  dir:direction port 
  * @retval none
  */
Motor::Motor(PwmOut *pwm, DigitalOut *dir){
    _pwm = pwm;
    _dir = dir;
    _pwm->period(PWM_PERIOD);
}

/**
  * @brief pwm output
  * @param  pwm(-100 +100)
  * @retval none
  */
void Motor::pwmout(float pwm){//-100,100
    *_dir = (pwm >= 0)? 1: 0;
    float val = pwm / 100.0f;
    if(val < 0) val = -val;
    *_pwm = val;
}


