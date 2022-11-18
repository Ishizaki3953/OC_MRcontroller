/**
  ******************************************************************************
  * @file           : InterruptInEx.cpp
  * @brief          : interrupt chattering prevention
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
#include "InterruptInEx.h"
#include "common.h"

/**
  * @brief read pin
  * @param  none
  * @retval pin value
  */
uint8_t InterruptInEx::read(){
    return _pin;
}
/**
  * @brief timer callback
  * @param  none
  * @retval none
  */
void InterruptInEx::t_callback(){
    if(_pin == 0) {
        _func.call();//push event
        if(_repeat) {
            _t.attach(this, &InterruptInEx::t_callback, 0.05);//0.02<= NG
        }else{
            _flg = false;//finish
        }
    } else {
        _func2.call();//pull event
        _flg = false;//finish
    }
}
/**
  * @brief constructor
  * @param  pin is pinName, repeat is event repeat flag 
  * @retval none
  */
InterruptInEx::InterruptInEx(PinName pin, bool repeat): _pin(pin){
    _repeat = repeat;
    _last_x = _pin;
    _flg = false;
}
/**
  * @brief loop
  * @param  none
  * @retval none
  */
void InterruptInEx::loop(){
    uint8_t x = _pin;
    if(_last_x){
        if(!x){
            if(!_flg){
                _t.attach(this, &InterruptInEx::t_callback, 0.01);
                _flg = true;
            }
        }
    }else{
        if(x){
            if(!_flg){
                _t.attach(this, &InterruptInEx::t_callback, 0.01);
                _flg = true;
            }
        }
    }
    _last_x = x;
}
/**
  * @brief attach
  * @param  function pointer
  * @retval none
  */
void InterruptInEx::attach(void (*func)(void)){
    _func.attach(func);
}
void InterruptInEx::attach2(void (*func)(void)){
    _func2.attach(func);
}
/**
  * @brief attach
  * @param  object and function pointer
  * @retval none
  */
template<typename T>
void InterruptInEx::attach(T *object, void (T::*func)(void)){
    _func.attach(object, func);
}
template<typename T>
void InterruptInEx::attach2(T *object, void (T::*func)(void)){
    _func2.attach(object, func);
}
