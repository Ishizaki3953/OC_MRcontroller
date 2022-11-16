/**
  ******************************************************************************
  * @file           : MotorEx.cpp
  * @brief          : Motor Class (TEST)
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
#include "MotorEx.h"
#include "common.h"

/**
  * @brief constructor
  * @param  pwm:pwm port  dir:direction port  period:timer period 
  * @retval none
  */
MotorEx::MotorEx(PwmOut *pwm, DigitalOut *dir, int period) : Motor(pwm, dir){
    TIM_PERIOD = period;
    _tim = 0;
    _tick.attach(this, &MotorEx::pwm_sync, PWM_PERIOD);
}
/**
  * @brief pwm output
  * @param  pwm 
  * @retval none
  */
void MotorEx::pwmout(float pwm){//-100,100
    dir = (pwm >= 0)? 1: 0;
    
    float val = pwm / 100.0f;
    if(val < 0) val = -val;
    *_pwm = val;
}
/**
  * @brief pwm sync interrupt
  * @param  none
  * @retval none
  */
void MotorEx::pwm_sync(){
    static int cnt = 0;
    if(++cnt == TIM_PERIOD){
        cnt = 0;
        _tim = !_tim;
        *_dir = dir ^ _tim; //tim is 1/0
    }
}
